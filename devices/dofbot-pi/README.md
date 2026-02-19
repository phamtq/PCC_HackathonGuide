# DOFBOT-Pi Guide

# Table of Contents
+ Hardware Setup
  + Power
  + Operating system
  + Connect peripherals
+ Connecting to the device
+ Controlling the arms
+ Open Computervision (OpenCV)

## Hardware Setup
To ensure that you don't run in any problems that might interrupt your design and programming, here are a few things to check to make sure your device is set up properly.

### Power
Ensure the red connector is plugged into the bottom black board where it says `DCIN 12V` and that the power adapter (brick) is connected to the wall. 

![Power connected to the board](images/powerconnector.png)

Make sure the switch is slid all the way to the left. The lights should turn on near OLED screen.

![Power switch](images/powerswitch.png)

The lights should turn on near OLED screen.

![OLED on](images/oledscreen.png)

If the OLED screen does not turn on, then the switch on the middle board (the one under the Raspberry Pi) is not switched to `ON`. 

**CAUTION**: If you hear rapid clicking noise immediately switch off the entire station. The rotation of the motors are trying to exceed it's current position leading to possible breakage. Either reset the position of the arms through programming or slowly (and carefully) move them manually, both methods with no power connected.

### Operating system

On Raspberry Pi, make sure the microSD card in inserted correctly (will feel like it's upside down). The image below is the correct way to load it.

![Correct way to load microSD card](images/sdcard_correct.png)
 
 The image below is the INCORRECT way to load the microSD card.
 
![Incorrect way to load microSD card](images/sdcard_incorrect.png)

### Connect peripherals
Plug in a micro-HDMI cable attached to a monitor and connect the keyboard and mouse. Make sure the USB cable for the camera is plugged in to the port shown below.

![USB port to connect camera](images/cameraport.png)

## Connecting to the device
You can connect to the device through WiFi and the SSH server set up on the Raspberry Pi. Using your laptop, connect to the wireless access pointer called `Dofbot`. The password for the WiFi access is `12345678` (the most secure of passwords :D).

Once you're connected, open up a terminal program on your computer like `Terminal` or PuTTy and use SSH to connect to the Raspberry Pi. The user acccount is called `pi`, the IP address is `192.168.1.11`, and the password is `yahboom`. 

WARNING: If you hear rapid clicking noise: Immediately switch off the entire station. Rotation trying to exceed limits, possible breakage.

## Controlling the arms
There are many ways to control the robot's motors and therefore it's arms. The easiest way to do so while still using your programming skills is through the language Python. Don't worry if you don't know that language! Here's a [simple guide](https://www.programming-idioms.org/cheatsheet/Cpp/Python) to help translate the C++ you know into Python. 

You're going to need the Arm_lib Python library in order to communicate with the robot. You can obtain that [here](https://pypi.org/project/Arm_lib/). These and many other libraries are like the `#include <iostream>` ones you use for your homework. 

You're going to need to use import `Arm_Device` from the `Arm_Lib` along with the time library. Place this at the beginning of your source code:

```py
import time
from Arm_Lib import Arm_Device
```

Next, you're going to have to create an `object` in order to control the motors. Think of the `object` like an instance of a class in C++ except you do it like:

```py
Arm = Arm_Devices()
```

The time library lets you put in delays (or sleep) so that things don't happen all at once. To use the `sleep` ability, you can type:

```py
time.sleep(1)
```

There are many functions of the `Arm_Device` library. The one below allows you to send the degree of rotation for all the motors (instead of calling the function for each, one at a time):

 ```py
Arm.Arm_serial_servo_write6(90, 90, 90, 90, 90, 90, 500)
time.sleep(1)
```

To learn what other functions are available to you, take a look at the `Arm_Lib` library you downloaded or click [here](files/Arm_Lib.py). Part of the fun of hackathons is figuring things out so play around with the various functions.

If all goes well, you can get your robot to do something simple like this:

[![It's alive](images/thumbnail.png)](videos/robot_moving.mp4) 

## 