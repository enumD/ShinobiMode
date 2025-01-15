# PeopleRecognition

This is the architecture of the Sentinel Mode for camper:

![arch](https://github.com/enumD/PeopleRecognition/blob/main/picture/arch.png)  

The sensor (or more) is placed under the camper and it's connected to a raspberry pi that pool the gpio every second to detect movement; the sensor keep the gpio high for 2 sec when movement is detected, the raspberry must wait for 3 sec after the first detenction to avoid multiple alert signal for one motion.  
Every time the sensor detect something the raspberry will send a notification via web socket to the laptop.  
The laptop will collect every alert and manage the strategy to use.
