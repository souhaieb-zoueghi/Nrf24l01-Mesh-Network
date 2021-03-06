# Nrf24l01-Mesh-Network

## 1. Introduction
This project is about developing a wireless sensors network for fire detection in fields, nodes measure several factors (temperature, humidity ...) and forward them either to the next node or to the gateway.

## 2. Description
The wireless communication is established through the Nrf24l01 module which operates on the 2.4GHz ISM band, if a node is down or off for unknown causes, its neighbor is capable of detecting that and will add some informations to its payload to inform the gateway about the issue.

## 3. Wireless Sensors Network Architecture
The transmitter measure multiple factors and forward them to the transceiver, which will receive the data and will sent it with its own sensed data to the gateway.

![](images/Network_Architecture.jpg)


## 4. Transmitter Down Case
 If the transmitter is down for some reasons, the transceiver will not receive any data, that's how the issue is detected and the transceiver will inform the gateway about it by adding the transmitter's id to its payload.

 ![](images/transmitter_down_case.jpg)


## 5. Transceiver Down Case
For each packet sent from the transmitter to the transceiver, an acknowledgement payload is exchanged to ensure that the transceiver is working properly, in case the transmitter don't receive  any ACK payload after certain tries, it will try to send its data directly to the gateway and it will add the transceiver's id to its payload to inform the gateway about the problem.
After reaching the gateway directly, the trasmitter will always try to establish a communication with the transceiver, if the issue is solved and it's back to work properly, we will automatically switch to our normal scenario (transmitter -> transceiver -> gateway)

![](images/transceiver_down_case.jpg)
