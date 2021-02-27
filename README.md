# TCP Temperature Monitor

## Background

TCP Temperature Monitor is a personal practice project written in C++ that helps me familiarize myself with the usage of Qt 5.

The goal of this project is to build a body temperature monitoring system that can be used at hospital or airport entrances for anti-epidemic purposes. The system visualizes the real-time temperature data sent from sensors and gives warnings when abnormal temperatures are detected.

<p align="center">
  <img src="/images/Temperature Monitor 2021_2_27 17_16_06.png" alt="Overview Picture" width="500">
</p>



## Description
This project consists of two parts: 

  1. Body temperature sensor simulator. This part is implemented in the [_"TCP_Sensor_Simulator"_](/TCP_Sensor_Simulator) folder. The simulator randomly generates a 16-bit number, representing the body temperature data in degrees Celsius, and then sends the data to the server using TCP/IP through the Qt Network module.
  
  2. Temperature monitor, implemented in the [_"TCP_Temperature_Monitor"_](/TCP_Temperature_Monitor
) folder. It sets up a server to collect temperature data from the sensor, and visualizes the data using Qt Widget and Charts. The program also gives audio warning when abnormal temp detected, by playing the pre-set alert from the Resource system through Qt Multimedia.


<p align="center">
  <img src="/images/Temperature Monitor 2021-02-27 17-25-02_Trim.gif" alt="Overview Picture" width="480">
</p>

> Displays realtime realtime data and alerts when abnormal data occurs

## Future Development
  1. Plan to have a function that memorizes the IP addresses of sensors, so that it's able to monitor temperature data from multiple sources.
  
  2. Plan to build a read/write system, so that history data can be stored in the disk and can be read and analyzed later in the future.
