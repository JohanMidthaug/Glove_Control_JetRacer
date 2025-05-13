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

    float getX() const;

    float getY() const;

    float getZ() const;

private:
    IMU& imu;

    float minX, maxX, minY, maxY, minZ, maxZ;

    float xPos = 300, yPos = 50, zPos = 500;

    float lastHeading = 0, lastPitch = 0, lastRoll = 0;

    float interval;
    unsigned long lastUpdate = 0;

    // Updated variables (help from GPT
    static constexpr float deadZone = 10.0f;
    static constexpr float gain = 0.001f; // How far one 'unit' moves the object, think this will be 1
    static constexpr float exponent = 1.5f;

    // Function for movement
    void driveAxis(float deltaDeg, float& pos);
    float angleDiff(float a, float b);
};


#endif //MOM_GLOVE_GESTERCONTROL_HPP
