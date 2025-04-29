//
// Created by Herman Hårstad Gran on 29/04/2025.
//

#ifndef MOM_GLOVE_GESTERCONTROL_HPP
#define MOM_GLOVE_GESTERCONTROL_HPP

#include <Arduino.h>
#include "Connections/IMU.hpp"
#include <Connections/FlexSensor.hpp>

class GestureControl{
public:

    GestureControl(IMU& imu_);

    void init();

    void track(FlexSensor& flexSensor);

    float getx();
    float gety();
    float getz();

private:
    IMU& imu;

    float minX, maxX, minY, maxY, minZ, maxZ;

    float x;
    float y;
    float z;

    double lastHeading, lastPitch, lastRoll;
};


#endif //MOM_GLOVE_GESTERCONTROL_HPP
