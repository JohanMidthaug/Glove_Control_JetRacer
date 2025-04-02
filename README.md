# AIS2104 Industrielle styrsystemer Project

This repository contains the code and configuration for the AIS2104 Industrielle styrsystemer project. The project is built using [PlatformIO](https://platformio.org/) and the Arduino framework for the ESP32 (Arduino Nano ESP32 board).

## Table of Contents
- [Overview](#overview)
- [Installation](#installation)


## Overview

This project is for controlling all the machines, I have provided a detailed description on how to initialize this project and contribute!

## Installation

### Prerequisites
- **PlatformIO:** Install PlatformIO IDE or the PlatformIO Core.
    - For the CLion, follow the instructions [here](https://docs.platformio.org/en/latest/core/installation.html).

### Clone the Repository

1. Open your terminal.
2. Clone the repository by running:

```bash
git clone git@github.com:HermanGran/Master-of-Machines.git
```

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