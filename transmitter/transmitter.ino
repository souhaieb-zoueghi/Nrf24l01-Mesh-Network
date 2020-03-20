#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>
#include <RF24_config.h>
#include <MQ2.h>
#include "dht.h"

#define DHT_PIN A1
#define MQ2_PIN A0

//nRF24L01 specifications
#define CE_PIN   9
#define CSN_PIN 10
#define RETRIES_DELAY 15   //maximum
#define RETRIES_NUMBER 15  //maximum
#define FAILURE_THRESHOLD 4

#define TRANSMITTER_ID 1
#define TRANSCEIVER_ID 2
#define TRANSCEIVER_NOT_DOWN 0
#define DATA_SIZE 6
#define DEBUG 0

RF24 radio(CE_PIN, CSN_PIN);

const byte pipes[][6] = {"1Node","2Node"};

float data_node1[DATA_SIZE];

MQ2 mq2(MQ2_PIN);

dht DHT;

int count = 0;

void setup() {

  Serial.begin(9600);

  mq2.begin();
  radio.begin();

  delay(100);

  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.stopListening();
  radio.openWritingPipe(pipes[0]);
  radio.setRetries(RETRIES_DELAY,RETRIES_NUMBER);

}

void loop() {

  float co = mq2.readCO();
  float smoke = mq2.readSmoke();
  delay(500);

  DHT.read11(DHT_PIN);
  delay(1000);
  float hum = DHT.humidity;
  float temp = DHT.temperature;

#if DEBUG == 1

  Serial.println(co);
  Serial.println(smoke);
  Serial.println(hum);
  Serial.println(temp);

#endif

  data_node1[0] = TRANSMITTER_ID;   //Node id
  data_node1[1] = co;
  data_node1[2] = smoke;
  data_node1[3] = hum;
  data_node1[4] = temp;
  data_node1[5] = TRANSCEIVER_NOT_DOWN;

  if(radio.write(&data_node1, sizeof(data_node1))){

    Serial.println("Ack payload received successfully");
    count = 0;

  }else{

    count++;

  }

  if(count > FAILURE_THRESHOLD){

    //the transceiver is down
    Serial.println("Failure !!!");
    Serial.println("the transceiver is down");

    //we must switch the writing pipe to send data directly to the gateway
    radio.stopListening();
    delay(200);
    radio.openWritingPipe(pipes[1]);
    //we put the id of the transceiver to inform the gateway that its down
    data_node1[5] = TRANSCEIVER_ID;

    radio.write(&data_node1, sizeof(data_node1));

    delay(200);
    //we switch the writing pipe to try to reach the transceiver every time
    radio.stopListening();
    delay(200);
    radio.openWritingPipe(pipes[0]);

}

}
