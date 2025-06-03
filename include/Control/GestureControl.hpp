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
     * @param track A bool value to toggle the update of the variables
     */
    void virtualJoystick(bool track);

    /**
     * Function for updating total orientation, not relative, but absolute orientation.
     * @param track A bool value to toggle the update of the variables
     */
    void orientation(bool track);

    /**
     * This function updates the height and the last joint angle in the kinematic chain, rZ
     * @param track A bool value to toggle the update of the variables
     */
    void heightControl(bool track);

    /**
     * Function for toggling the gripper for the robot, taking two bool values. Using two flex sensor for toggling
     * @param heigt Bool value that controls the height and last joint angle
     * @param track Bool value that controls the position adjustment
     * @return Bool value to toggle the gripper. Has a 600 mS debounce rate
     */
    String toggleGripper(bool heigt, bool track);

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
     * @return Rotation along z axis
     */
    float getRZ() const;

    /**
     * Setter function for x position
     * @param pos New x position
     */
    void setX(float pos);

    /**
     * Setter function for y position
     * @param pos New y position
     */
    void setY(float pos);

    /**
     * Setter function for z position
     * @param pos New z Position
     */
    void setZ(float pos);

    /**
     * Forward kinematic solver, solving one instance, given the joint angles. Triggering on falling edge
     * @param q double array of 6 joint angles
     * @param trigger Bool value for trigger
     */
    void computeForwardKinematics(const double q[6], bool trigger);

private:
    IMU& imu;

    float maxRZ = PI, minRZ = -PI;

    float xPos = 250, yPos = -400, zPos = 150;
    float rZ = 1.83;

    float lastHeading = 0, lastPitch = 0, lastRoll = 0;
    float lastHeightHeading = 0, lastHeightPitch = 0;

    float interval;
    unsigned long lastUpdate = 0;

    // Updated variables (help from GPT)
    static constexpr float deadZone = 8.0f;
    static constexpr float gain = 0.001f; // How far one 'unit' moves the object, think this will be 1
    static constexpr float exponent = 1.2f;

    // Function for movement
    void driveAxis(float deltaDeg, float& pos);
    void driveAxisOrientation(float deltaDeg, float &pos);
    float angleDiff(float a, float b);
    float processAxisDelta(float delta, float currentValue, float maxValue);

    // In your constructor:
    float baseGain = 0.3f;    // Start low, increase as needed
    float maxLength = 850.0f; // Your robot's reach
    float lastAngle; // Add to your class
    bool prevTrigger;      // the state just before trigger was updated
};


#endif //MOM_GLOVE_GESTERCONTROL_HPP
