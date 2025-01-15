## 1. HC-SR501  
Main Features:
This is one of the most popular and affordable PIR sensors.
Good detection range (up to about 7 meters with a 120° detection zone).
Has a digital output, allowing you to read the sensor's state (high or low).
Powered by 5V DC.
Easy to use with Raspberry Pi or Arduino.
Pros: Affordable, widely used, easily available.

Cons: Limited range, sensitivity not precisely adjustable (but can be adjusted via a potentiometer on the module).

Where to Buy: Amazon, AliExpress, or online electronics stores.

$ 3 euro each

## 2. AM312  ![Sensor specs](https://github.com/enumD/PeopleRecognition/blob/main/picture/RCWL_0516_specs_02.png)  
Main Features:

A miniaturized PIR sensor compared to other models.
Compact and energy-efficient.
Detects motion up to 3-5 meters.
Excellent for applications in tight spaces.
Pros: Compact and low power consumption.


Cons: Shorter detection range compared to models like the HC-SR501.

Where to Buy: Amazon, AliExpress.

$ 2-3 euro each

## 3. PIR Motion Sensor Module (KY-022)  
Main Features:

Low-cost module.
Detects motion up to 5-7 meters.
Sensitivity adjustable via a potentiometer.
Easy to integrate with microcontrollers like Arduino and Raspberry Pi.
Pros: Adjustable sensitivity, good detection range.

Cons: Sensitivity can be variable depending on orientation and temperature.

Where to Buy: Amazon, AliExpress, or online electronics stores.

$ 4 euro each

## 4. RCWL-0516 (Microwave Sensor)   

Main Features:

This sensor is not a PIR sensor but a microwave sensor that detects motion using microwaves.
Has a much longer detection range (up to 10-15 meters).
Works even in low light conditions and is not affected by temperature.
Adjustable sensitivity.
Pros: Longer range than PIR sensors, unaffected by temperature.

Cons: Variable sensitivity, may be more expensive than a PIR sensor.

Where to Buy: Amazon, AliExpress, electronics stores.

$ 2.5 euro each and maybe it's not necessary to point directly 

## 5. Panasonic EKMC1601111  
Main Features:

High-sensitivity PIR sensor.
Good detection range (up to 8 meters).
Used for industrial and commercial applications.
Easily integrated with microcontrollers like Arduino and Raspberry Pi.
Pros: High sensitivity and good range.

Cons: More expensive than basic PIR sensors like HC-SR501.

Where to Buy: Amazon, electronics distributors like Digi-Key or Mouser.

$ 10-11 euro each


# Which one?  
The camper is outside: sun, rain and atmosferic agent represent an hard environment.  
PIR sensors suffer (src chatgpt):  
**Direct sunlight**: can cause a lot of false positives or false negatives because the sunlight can interfere with the sensor’s ability to distinguish between the heat emitted by a person and the heat of surrounding objects (such as the ground or the vehicle).   
**Thermal Interference**:In some cases, a hot surface (like a sun-heated car or camper) can emit enough infrared radiation that the PIR sensor can't distinguish it from a person, leading to incorrect readings.  

PIR sensors suffer a lot with direct sunligth, i can install the sensors on the external roof of the camper and point them down the sides, this could reduce the possibility to have direct sunlight throught them.  
Another option could be to install the sensors under the camper, under the bumper or better inside the bumper like the car's professional ones. With the microwave sensor maybe i can directly place them behind the bumper to make them not visible at all.
My choice is going in the direction of the microwave sensor:

## RCWL-0516 (Microwave Sensor)  
![Sensor specs](https://github.com/enumD/PeopleRecognition/blob/main/picture/RCWL_0516_specs.png)  

### Pinout  
The RCWL0516 module is a single breakout board with the following connections:  
3V3 : it is the "output" from the onboard 3.3V regulator which can be used to power external circuits. Remember, this is not an input pin. This pin can provide up to 100mA of current.  
GND : is the ground pin.  
OUT : is the 3.3V TTL logic output. This pin goes HIGH for 2seconds when a motion is detected and goes LOW when no motion is detected. The output of this module is "analog" and can be connected to an analog input of a microcontroller and sampled by an ADC. The output voltage is roughly proportional to the distance between the sensor and the object.   
VIN : provides power to the module. Connect this pin to an input voltage anywhere between 4 to 28V (however, 5V is commonly used). This module consumes less than 3mA of current so, you can easily power this by the 5V output from an Arduino or a Raspberry Pi.  
CDS : pins are where you attach an optional LDR (light dependent resistor) allowing it to operate only in the dark.  

![Sensor specs](https://github.com/enumD/PeopleRecognition/blob/main/picture/RCWL_0516_specs_02.png)  

### Specs  
The MMBR941M RF amplifier is a high-speed NPN transistor "Q1" that takes low-power RF signal and boosts it to a higher power level. The antenna is integrated on the PCB. It has a detection range of approximately "7 Meters" while only consuming less than "3mA of current". When triggered, the output (OUT) pin will switches from LOW (0V) to HIGH (3.3V) for 2 to 3 seconds before returning to its idle (LOW) state.  
The transistor Q1 also acts as a mixer that combines the transmitted and received signal and outputs the difference which is filtered by the low pass filter formed by C9 and R8, and is amplified by the IC.   

### Jumper Tuning  
![Sensor specs](https://github.com/enumD/PeopleRecognition/blob/main/picture/RCWL_0516_specs_03.png)  

The module has 3 jumper settings at the back of it. The sensors default settings can be altered, by populating these jumpers with appropriate resistors and capacitors:  

**C-TM** : (Pulse length Adjustment) By installing a suitable SMD capacitor you can adjust the repeat trigger time by extending the output pulse length. Default trigger time is 2s. Increasing capacitor's capacity will make repeat trigger time longer. A 0.2µF capacitor extends the output pulse to 50s, while 1µF extends it to 250s.  
**R-GN** : (Detection Range Adjustment) By installing a suitable resistor you can reduce the detection range. The default detection range is 7m. If you install a 1M resistor the distance reduces to 5m, while a 270K resistor reduces it to 1.5m.  
**R-CDS** : (Light Sensitivity Adjustment) You can use this as an alternative to soldering the LDR. Any resistor between 47K – 100K will suffice. The lower the value, the brighter the light must be in order to disable the trigger.  

Here are two images, the first with two sensors with 2.5 m range and the other with one sensor with 3.6 m range: camper size is 2,2 m wide and 5.62 m long

![2500 mm](https://github.com/enumD/PeopleRecognition/blob/main/picture/2500mm.png)  

![3600 mm](https://github.com/enumD/PeopleRecognition/blob/main/picture/3600.png)  

To obtain these ranges i should use 500 KOhm for 2.5 m and a 720KOhm for 3.6 m. 

I figured out that i cannot place the sensors below the camper because it's not safe when driving. It would be cool to place some linear actuator to automatize the movement but its too expensive for me, i must install the sensors on the sides or on the roof and face them downward, using the holes of the solar panel for the cables. I have to design a box to protect the sensor and to limit the window of detection to point to the ground. foil paper can be used to block unwanted directions. 


