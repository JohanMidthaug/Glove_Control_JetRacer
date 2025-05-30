//
// Created by Herman Hårstad Gran on 29/04/2025.
//
#include "Control/GestureControl.hpp"
#include <cmath>

// Constructor
GestureControl::GestureControl(IMU& imu_):
imu(imu_),
minX(-850),
minY(-850),
minZ(-850),
maxX(850),
maxY(850),
maxZ(850),
interval(100) {}

// Init function, used in setup
void GestureControl::init() {}

// Virtual joystick function
void GestureControl::virtualJoystick(FlexSensor& flexSensor) {
    if (flexSensor.read()) {

        float roll = imu.getGestureRoll();
        float pitch = imu.getGesturePitch();
        float heading = imu.getGestureHeading();

        // Signed deltas from the reference we stored earlier
        float dRoll    = roll    - lastRoll;
        float dPitch   = pitch   - lastPitch;
        float dHeading = angleDiff(heading, lastHeading);

        /*
        Serial.print("Delta | roll: ");
        Serial.print(dRoll);
        Serial.print(" pitch: ");
        Serial.print(dPitch);
        Serial.print(" heading: ");
        Serial.println(dHeading);
        */

        // 3. Update virtual position for each axis
        driveAxis(dHeading,    xPos);
        driveAxis(dPitch,   yPos);

    } else {
        lastHeading = imu.getGestureHeading();
        lastPitch = imu.getGesturePitch();
        lastRoll = imu.getGestureRoll();
        /*
        Serial.print("Delta | roll: ");
        Serial.print(lastRoll);
        Serial.print(" pitch: ");
        Serial.print(lastPitch);
        Serial.print(" heading: ");
        Serial.println(lastHeading);
         */
    }
}

void GestureControl::orientation(FlexSensor &flexSensor) {
    if (flexSensor.read()) {
        rX = imu.roll();
        rY = imu.pitch();
        rZ = imu.heading();
    }
    Serial.print("Orientation | rX: ");
    Serial.print(imu.roll());
    Serial.print(" rY: ");
    Serial.print(imu.pitch());
    Serial.print(" rZ: ");
    Serial.println(imu.heading());
}

float GestureControl::getX() const {
    return xPos;
}

float GestureControl::getY() const {
    return yPos;
}

float GestureControl::getZ() const {
    return zPos;
}

float GestureControl::getRX() const {
    return rX;
}

float GestureControl::getRY() const {
    return rY;
}

float GestureControl::getRZ() const {
    return rZ;
}

void GestureControl::driveAxis(float deltaDeg, float &pos) {
    float mag = fabsf(deltaDeg);

    // 1. DEAD-ZONE
    if (mag < deadZone)
        return;                          // ignore tiny motions

    // 2. Remove the dead-zone so the curve starts at zero
    mag -= deadZone;

    // 3. Non-linear response (exponential)
    float step = gain * powf(mag, exponent);

    // 4. Apply the sign to preserve direction
    pos += copysignf(step, deltaDeg);
}

// Keep heading differences inside −180 … +180 so wrap-around at 0/360 behaves.
float GestureControl::angleDiff(float a, float b)
{
    float d = fmodf(a - b + 540.0f, 360.0f) - 180.0f;
    return d;   // signed shortest-arc difference
}

