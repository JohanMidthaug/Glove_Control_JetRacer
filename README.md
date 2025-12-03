# Virtual Joystick JetRacer

This repository is forked from Master-of-Machines. Master-of-Machines was a project developed in the course AIS2104, for virtual joystick control of a UR5e robot. 
Most of the code is reused and expanded to manually control a JetRacer ROS AI using a virtual joystick.
The project is built using [PlatformIO](https://platformio.org/) and the Arduino framework for the ESP32 (Arduino Nano ESP32 board). 
This project takes use of the 9-DoF IMU (Inertial Measurement Unit) to track the orientation of the hand, and a way of 
creating a "Virtual Joystick" to control manipulate a 6-DoF Robot Arm. It uses flex sensors to toggle tracking of the 
virtual joystick and orientation, and publishes it via MQTT to a broker. 

## Table of Contents
- [Overview](#overview)
- [Installation](#installation)


## Overview

This project is for controlling JetRacer ROS AI via MQTT, and what follows next is a detailed description on how to initialize this project.

**Note:**  
This project depends on a separate project which has to run on the physical JetRacer.
This project can be found in jetracer_control.zip, and has a separate README file.
That project handles:
- Publishing the JetRacer’s camera stream.
- Publishing runtime data, such as runtime, odometry and IMU data.
- Receiving MQTT control commands and forwarding them to the motors.
  

## Installation

### Prerequisites
- **PlatformIO:** Install PlatformIO IDE or the PlatformIO Core.
    - For the CLion, follow the instructions [here](https://docs.platformio.org/en/latest/core/installation.html).

### PlatformIO setup
To compile and build the project, run the following:

 ```bash
pio init --ide clion
```

Then run

```bash
pio run
```

To open the serial monitor, run:

```bash
pio device monitor 
```
