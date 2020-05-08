# Nrf24l01-Mesh-Network

## 1. Introduction
This project is about eveloping a wireless sensors network for fire detection in fields, nodes measure several factors (temperature, humidity ...) and forward them either to the next node or to the Gateway.

## 2. Description
The wireless communication is established through the Nrf24l01 module which operates in the 2.4Ghz ISM band, if a node is down or off for unknown causes it's neighbor is capable of detecting that and will add some informations to it's payload to inform the Gateway about the issue.

## 3. Wireless Sensors Network Architecture
![](images/network_architecture.jpg)
