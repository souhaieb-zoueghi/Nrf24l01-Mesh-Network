#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <RF24/RF24.h>
#include <fstream>
#include <ctime>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#define TIMEOUT 30
#define HOST "database_server_ip_address"
#define USERNAME "database_username"
#define PASSWORD "user_password"
#define RETRIES_DELAY 15   //maximum
#define RETRIES_NUMBER 15  //maximum

using namespace std;

int main()
{

  RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_16MHZ);

  const uint8_t pipe[6] = {"2Node"};

  //variable for storing received data
  float data[6] = {0};

  //variables for formating sql statements
  char update_temperature[50] = {0};
  char update_humidity[50] = {0};
  char update_status[50] = {0};
  char update_smoke[50] = {0};

  //Nrf24l01 configuration
  radio.begin();
  radio.setAutoAck(true); // Ensure autoACK is enabled
  radio.setRetries(RETRIES_DELAY,RETRIES_NUMBER);
  radio.enableDynamicPayloads();
  radio.openReadingPipe(1,pipe);
  radio.startListening();

  //variables for detecting when we are exceeding the timeout
  time_t current_time, started_waiting_at;
  int time_waited = 0;

  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;

  driver = get_driver_instance();
  con = driver->connect(HOST, USERNAME, PASSWORD); //IP Address, user name, password

  stmt = con->createStatement();
  stmt->execute("USE data_base_name");

  started_waiting_at = time(NULL);

	while(1)
	{

		if(radio.available())
		{
			while(radio.available())
			{
				radio.read( &data, sizeof(data));
		        }


			cout << "Data received from node " << data[0] << endl;
			cout << "Co : " << data[1] << " ppm" << endl;
			cout << "Smoke : " << data[2] << " ppm" << endl;
			cout << "Humidity : " << data[3] << "%" << endl;
			cout << "Temperature : " << data[4] << " C" << endl;

                        sprintf(update_temperature, "update capteur set temperature = %d where id=%d", (int)data[4], (int)data[0]);
		        sprintf(update_humidity, "update capteur set humidity = %d where id=%d", (int)data[3], (int)data[0]);
			sprintf(update_smoke, "update capteur set gaz = %d where id=%d", (int)data[2], (int)data[0]);
			sprintf(update_status, "update capteur set is_down = 0 where id=%d", (int)data[0]);

			stmt->execute(update_temperature);
			stmt->execute(update_humidity);
			stmt->execute(update_smoke);
			stmt->execute(update_status);

			if(data[5] != 0)
			{
				cout << "Node " << data[5] << " is down !!!!" << endl;
				sprintf(update_status, "update capteur set is_down = 1 where id=%d", (int)data[5]);
				stmt->execute(update_status);
		        }

	                started_waiting_at = time(NULL);

		}

		current_time = time(NULL);
		time_waited = current_time - started_waiting_at;

		if(time_waited > TIMEOUT)
		{
				cout << "**************nodes are down ************" << endl;
				started_waiting_at = time(NULL);
				stmt->execute("update capteur set is_down = 1 where id=1");
				stmt->execute("update capteur set is_down = 1 where id=2");
		}

	}

	return 1;

}
