//
// Created by Herman Hårstad Gran on 29/04/2025.
//

#ifndef MOM_GLOVE_GESTERCONTROL_HPP
#define MOM_GLOVE_GESTERCONTROL_HPP

#include <Arduino.h>
#include "Connections/IMU.hpp"
#include <Connections/FlexSensor.hpp>

/**
 * Class for gesture control sensor, like a virtual joystick
 */
class GestureControl{
public:

    /**
     * Constructor
     * @param imu_ An instance of the imu class
     */
    GestureControl(IMU& imu_);

    /**
     * Init function, used in setup
     */
    void init();

    /**
     * Virtual joystick function for tracking relative position from start of flex sensor
     * @param flexSensor An instance of flex sensor class, used for toggling of tracking
     */
    void virtualJoystick(FlexSensor& flexSensor);

    /**
     * Function for updating total orientation, not relative, but absolute orientation.
     * @param flexSensor An instance of flex sensor class, used for toggling
     */
    void orientation(FlexSensor& flexSensor);

    /**
     * Getter function
     * @return X position
     */
    float getX() const;

    /**
     * Getter function
     * @return Y position
     */
    float getY() const;

    /**
     * Getter function
     * @return Z pos
     */
    float getZ() const;

    /**
     * Getter function
     * @return Rotation along x axis
     */
    float getRX() const;

    /**
     * Getter function
     * @return Rotation along y axis
     */
    float getRY() const;

    /**
     * Getter function
     * @return Rotation along z axis
     */
    float getRZ() const;

private:
    IMU& imu;

    float minX, maxX, minY, maxY, minZ, maxZ;

    float xPos = 300, yPos = 50, zPos = 100;
    float rX = 3.14, rY = 0, rZ = 1.83;

    float lastHeading = 0, lastPitch = 0, lastRoll = 0;

    float interval;
    unsigned long lastUpdate = 0;

    // Updated variables (help from GPT)
    static constexpr float deadZone = 10.0f;
    static constexpr float gain = 0.001f; // How far one 'unit' moves the object, think this will be 1
    static constexpr float exponent = 1.5f;

    // Function for movement
    void driveAxis(float deltaDeg, float& pos);
    float angleDiff(float a, float b);
};


#endif //MOM_GLOVE_GESTERCONTROL_HPP
