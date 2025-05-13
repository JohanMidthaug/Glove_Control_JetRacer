//
// Created by Herman Hårstad Gran on 08/04/2025.
//

#ifndef MOM_GLOVE_IMU_HPP
#define MOM_GLOVE_IMU_HPP

#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "utility/imumaths.h"
#include <vector>

class IMU{
public:
    IMU();

    void init();

    void run();

    float heading();

    float pitch();

    float roll();

    float getGestureHeading();
    float getGesturePitch();
    float getGestureRoll();

    Adafruit_BNO055& getbno();

private:
    Adafruit_BNO055 bno;

    float absHeading;
    float absPitch;
    float absRoll;

    float gestureHeading;
    float gesturePitch;
    float gestureRoll;
};

#endif //MOM_GLOVE_IMU_HPP
