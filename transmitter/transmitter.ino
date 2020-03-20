#include <nRF24L01.h>
//#include <printf.h>
#include <RF24.h>
#include <SPI.h>
#include <RF24_config.h>
#include <MQ2.h>
#include "dht.h"

#define dht_apin A1

#define DEBUG 1

RF24 radio(9, 10);

const byte addresses[][6] = {"1Node","2Node","3Node"};

float data[5];

int windSpeed = 0, sensorValue;

MQ2 mq2(A0);

int ack = 0;

int count = 0;

void setup() {
  
  // put your setup code here, to run once:
Serial.begin(9600);
mq2.begin();
radio.begin();
delay(100);
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.stopListening();
  radio.openWritingPipe(addresses[0]);
  radio.setRetries(15,15);
}

float co, smoke, hum, temp;

//const char text[] = "souhaib";
dht DHT;

void loop() {

  co = mq2.readCO();
  smoke = mq2.readSmoke();
  delay(500);
  
  DHT.read11(dht_apin);
  delay(1000);
  hum = DHT.humidity;
  temp = DHT.temperature;

 /*sensorValue = analogRead(A3);
 sensorValue = sensorValue * (5.0 / 1023.0);
 delay(500);*/

  //windSpeed = getWindSpeed();

#if DEBUG == 1
 Serial.println(sensorValue);
  Serial.println(windSpeed);
  #endif

  
Serial.println(co);
Serial.println(smoke);
Serial.println(hum);
Serial.println(temp);

  data[0] = 1;
  data[1] = co;
  data[2] = smoke;
  data[3] = hum;
  data[4] = temp;
  //data[5] = windSpeed;
  
  
  radio.write(&data, sizeof(data),0);
  
//delay(1000);

//radio.startListening();

if(radio.isAckPayloadAvailable()){
  
  radio.read(&ack,sizeof(int));
  Serial.println("received ack payload is : ");
  Serial.println(ack);
  count = 0;
  
}

else{
  Serial.println("Failure !!!");
  count++;
}

if(count >= 10){
  
  Serial.println("Node 2 is down");
  radio.stopListening();
  delay(200);
  radio.openWritingPipe(addresses[1]);
  data[0] = 3;

  radio.write(&data, sizeof(data));


  delay(200);
  radio.stopListening();
  delay(200);
  radio.openWritingPipe(addresses[0]);


}





/*

unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 500000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }

    if ( timeout ){                                             // Describe the results
        Serial.println("Failed, response timed out.");
    }else{
        Serial.println("Packet sent successfully");
}*/
}
/*
int getWindSpeed(){
if((sensorValue >= 0) && (sensorValue < 10))
  return 0;
  
if((sensorValue) >= 10 && (sensorValue <= 30))
  return 10;
   
if((sensorValue) > 30 && (sensorValue <= 70))
  return 20;

if((sensorValue > 70) && (sensorValue <= 100))
  return 30;

return 40; 
}*/
