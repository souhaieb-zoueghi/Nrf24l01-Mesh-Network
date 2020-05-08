# Nrf24l01-Mesh-Network

## 1. Introduction
This project is about eveloping a wireless sensors network for fire detection in fields, nodes measure several factors (temperature, humidity ...) and forward them either to the next node or to the Gateway.

## 2. Description
The wireless communication is established through the Nrf24l01 module which operates in the 2.4Ghz ISM band, if a node is down or off for unknown causes it's neighbor is capable of detecting that and will add some informations to it's payload to inform the Gateway about the issue.

## 3. Wireless Sensors Network Architecture
![](images/Network_Architecture.jpg)
The transmitter measure multiple factors and forward them to the transceiver, which will receive the data and will sent it with it's own sensed data to the Gatway.

## 4. Transmitter Down Case
 ![](images/transmitter_down_case.jpg)
 If the transmitter is down for some reasons, the transceiver is capable of detecting that and it will inform the gateway about this issue by adding the transmitter's id to it's payload.
