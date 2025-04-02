# Shinobi Mode

This is the main functionalities of Shinobi:

![arch](https://github.com/enumD/PeopleRecognition/blob/main/picture/arch.png)  

### **Camera Stream:**
Stream the cameras connected to the device. Used while driving as a parking camera or to increase the view during car manovre.
Could be used from telegram in order to control the webcam from remote.

### **Motion Detection:**
The sensors are placed under the camper, repaired from sun, rain etc; probably glued or somethig behind the bumper.  
When alarm modes are activated the raspberry will pool the sensors in order to detect movement near the camper (PIR or Microwave sensors?). Once the movement is detected, based on the alarm mode selected, the raspberry could start to try to recognize people and avoid false alarm (animals, cars passing by etc) or start to track them in order to extablish the amount of time they spent near and take actions like: Telegram notification with images, stream, play alarm sound, record images etc


