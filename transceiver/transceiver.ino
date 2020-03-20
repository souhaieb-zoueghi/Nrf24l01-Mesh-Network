#include <RF24.h>
#include <SPI.h>
#include <MQ2.h>
#include "dht.h"

#define DHT_PIN A1
#define MQ2_PIN A0

//nrf24l01 specifications
#define CE_PIN 9
#define CSN_PIN 10
#define RETRIES_DELAY 15   //maximum
#define RETRIES_NUMBER 15  //maximum
#define FAILURE_THRESHOLD 4

#define TRANSMITTER_ID 1
#define TRANSCEIVER_ID 2
#define TRANSMITTER_NOT_DOWN 0
#define DATA_SIZE 6

RF24 radio(CE_PIN, CSN_PIN);

const byte pipes[][6] = {"1Node","2Node"};

MQ2 mq2(MQ2_PIN);

dht DHT;

float transmitter_data[DATA_SIZE] = {0};
float transceiver_data[DATA_SIZE] = {0};

int count  = 0;

void setup() {

  Serial.begin(9600);

  mq2.begin();

  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.openReadingPipe(1, pipes[0]);
  radio.openWritingPipe(pipes[1]);
  radio.setRetries(RETRIES_DELAY, RETRIES_NUMBER);
  radio.startListening();

}

void loop() {

  float co = mq2.readCO();
  float smoke = mq2.readSmoke();

  DHT.read11(DHT_PIN);
  delay(1000);
  float hum = DHT.humidity;
  float temp = DHT.temperature;

  transceiver_data[0] = TRANSCEIVER_ID;
  transceiver_data[1] = co;
  transceiver_data[2] = smoke;
  transceiver_data[3] = hum;
  transceiver_data[4] = temp;

  #if DEBUG == 1

  Serial.println("Data of this node: ");
  Serial.println(transceiver_data[0]);
  Serial.println(transceiver_data[1]);
  Serial.println(transceiver_data[2]);
  Serial.println(transceiver_data[3]);
  Serial.println(transceiver_data[4]);

  #endif

 if(radio.available()){

   transceiver_data[5] = TRANSMITTER_NOT_DOWN;

  while (radio.available()){

    radio.read(&transmitter_data, sizeof(transmitter_data));

  }

  #if DEBUG == 1

  Serial.println("Data received from transmitter : ");
  Serial.println(transmitter_data[0]);
  Serial.println(transmitter_data[1]);
  Serial.println(transmitter_data[2]);
  Serial.println(transmitter_data[3]);
  Serial.println(transmitter_data[4]);

  #endif

  radio.stopListening();

  radio.write(&transmitter_data, sizeof(transmitter_data));
  radio.write(&transceiver_data, sizeof(transceiver_data));

  radio.startListening();

  count = 0;

 }else{

  count++;

 }

 if(count >= FAILURE_THRESHOLD){

  radio.stopListening();

  transceiver_data[5] = TRANSMITTER_ID;

  radio.write(&transceiver_data, sizeof(transceiver_data));
  radio.startListening();

 }

 }
