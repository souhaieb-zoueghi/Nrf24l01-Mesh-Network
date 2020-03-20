//#include <nRF24L01.h>
//#include <printf.h>
#include <RF24.h>
#include <SPI.h>
#include <RF24_config.h>
#include <MQ2.h>
#include "dht.h"

#define dht_apin A1

RF24 radio(9, 10);

byte addresses[][6] = {"1Node","2Node","3Node"};
//byte pipes[][6] = {{0xE8, 0xE8, 0xF0, 0xF0, 0xE1}, {0xF0, 0xF0, 0xF0, 0xF0, 0xE1}};


MQ2 mq2(A0);


tyygg

float data[5] = {0};
float data1[5] = {0};
int ack = 1;
int count  = 0;

void setup() {
  
  // put your setup code here, to run once:
Serial.begin(9600);
radio.begin();
delay(100);
  radio.setAutoAck(true);
  radio.enableAckPayload();
  //radio.setChannel(0x76);
  radio.enableDynamicPayloads();
  radio.openReadingPipe(1, addresses[0]);
  radio.openWritingPipe(addresses[1]);
  radio.setRetries(15,15);
  radio.startListening();

}

dht DHT;
float co, smoke, hum, temp;

void loop() {
  
  co = mq2.readCO();
  smoke = mq2.readSmoke();
  delay(1000);
  DHT.read11(dht_apin);
  delay(1000);
  hum = DHT.humidity;
  temp = DHT.temperature;
  data1[0] = 2;
  data1[1] = co;
  data1[2] = smoke;
  data1[3] = hum;
  data1[4] = temp;
  

    
 if(radio.available())
 {
  radio.writeAckPayload( 1, &ack, sizeof(int) );
  while (radio.available())
  {
 
      radio.read(&data, sizeof(data));
      
  }

  Serial.println("Data received from ndoe 1 : ");
  Serial.println(data[0]);
  Serial.println(data[1]);
  Serial.println(data[2]);
  Serial.println(data[3]);
  Serial.println(data[4]);
  
    delay(1000);

 Serial.println("Data of this node: ");
  Serial.println(data1[0]);
  Serial.println(data1[1]);
  Serial.println(data1[2]);
  Serial.println(data1[3]);
  Serial.println(data1[4]);
    
    //radio.stopListening();

    /*radio.openWritingPipe(addresses[3]);

    radio.write(&ack, sizeof(ack);*/

   delay(1000);

      radio.stopListening();

  radio.write(&data, sizeof(data));
  radio.write(&data1, sizeof(data1));

  delay(1000);

  radio.startListening();

  count = 0;
    
 }else{
  
  count++;
  
 }

 if(count >= 10){

  delay(1000);

  radio.stopListening();

  data1[0] = 4;
  
  radio.write(&data1, sizeof(data1));
  
  delay(1000);

  radio.startListening();
  
 }




 }
 // put your main code here, to run repeatedly:
