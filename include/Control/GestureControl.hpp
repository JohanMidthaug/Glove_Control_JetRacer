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

    float getX();

    float getY();

    float getZ();

private:
    IMU& imu;

    float minX, maxX, minY, maxY, minZ, maxZ;

    double xPos;
    double yPos;
    double zPos;

    double lastHeading, lastPitch, lastRoll;

    float interval;
    unsigned long lastUpdate;

    // Updated variables (help from GPT
    static constexpr float deadZone = 3.0f;
    static constexpr float gain = 0.001f; // How far one 'unit' moves the object, think this will be 1
    static constexpr float exponent = 2.2f;

    // Function for movement
    void driveAxis(float deltaDeg, double& pos);
    float angleDiff(float a, float b);
};


#endif //MOM_GLOVE_GESTERCONTROL_HPP
