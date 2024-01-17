# 3D Noughts And Crosses on Arduino+LEDCube4x4x4

DESCRIPTION:
3D Noughts and Crosses game for up to 3 players. 
Presented on 4x4x4 RGB LED cube controlled with Arduino Micro and self designed extension board. 
Users play with use of Android app communicating with LED cube controller over Bluetooth 2.1. 

Hardware - designed and produced PCB extension board  for integrating device sub-modules:
  - self-designed LEDs actuation circuitry (D-latches, current drivers and layer multiplexing)
  - self-made 4x4x4 RGB LED cube
  - Arduino Micro module, 
  - Bluetooth module, 
  - DC-DC PMIC, 

Firmware (Arduino control loop):
  - C-style embedded software for Arduino (.ino file compiled with AVR-GCC)

Software (Android mobile app):
  - User interface implemented as wireless mobile app for Android
  - No Code approach - app generated in MIT App Inventor


Repository content:
 - Arduino firmware files,
 - Android app MIT App Inventor 2 project file.

Some images of constructed controller board, LED cube and app: 
- ![controllerboard](https://user-images.githubusercontent.com/43730948/160214869-a0a5f5eb-0d04-48aa-9ff5-5a2e40e3c3ff.jpg)
- ![pcb_design](https://github.com/jakub-zwiefka/3d_noughts_and_crosses/assets/43730948/1367aa74-1437-4734-9ef9-3983df774003)
- ![ledcube](https://user-images.githubusercontent.com/43730948/160214873-4da97b2e-2d5e-4119-9f47-c1a870f3df76.jpg)
- ![app1](https://user-images.githubusercontent.com/43730948/160214878-c71dcdf1-2943-47ea-8c20-ddb3c2511df2.png)
- ![app2](https://user-images.githubusercontent.com/43730948/160215259-6415e702-27a0-4571-998c-cac66b56dc7c.png)
- ![rgb3d_system](https://user-images.githubusercontent.com/43730948/173565201-96dc5623-3f65-4ee9-93d0-13e5f1e7c692.jpg)
