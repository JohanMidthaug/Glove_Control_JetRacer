//
// Created by Herman Hårstad Gran on 29/04/2025.
//
#include "Control/GestureControl.hpp"
#include <cmath>

// Constructor
GestureControl::GestureControl(IMU& imu_): imu(imu_), interval(100) {}

// Init function, used in setup
void GestureControl::init() {}

// Virtual joystick function, got a little help from GPT for creating polar coordinates update for IMU control.
// Makes it more intuitive
void GestureControl::virtualJoystick(bool track) {
    if (track) {
        float dHeading = imu.getGestureHeading() - lastHeading;
        float dPitch = imu.getGesturePitch() - lastPitch;

        float currentLength = sqrtf(xPos * xPos + yPos * yPos);
        float currentAngle = atan2f(yPos, xPos) * 180.0f / M_PI;

        // Stabilize angle if near center
        if (currentLength < 0.05f) {
            currentAngle = lastAngle;
        } else {
            lastAngle = currentAngle;
        }

        // Length update (pitch)
        currentLength += processAxisDelta(dHeading, currentLength, maxLength);
        currentLength = constrain(currentLength, 0.0f, maxLength); // Clamp length

        // Angle update (roll) — only if meaningful
        if (currentLength > 0.05f) {
            float angleStep = processAxisDelta(dPitch, currentAngle, 180.0f);
            angleStep *= (currentLength / maxLength); // Reduce angle influence near center
            currentAngle += angleStep;
        }

        float radAngle = currentAngle * M_PI / 180.0f;
        xPos = currentLength * cosf(radAngle);
        yPos = currentLength * sinf(radAngle);
    } else {
        lastHeading = imu.getGestureHeading();
        lastPitch = imu.getGesturePitch();
    }
}

// Orientation
void GestureControl::orientation(bool track) {
    if (track) {
        rX = imu.roll();
        rY = imu.pitch();
        rZ = imu.heading();
    }
}

// Function for height control
void GestureControl::heightControl(bool track) {
    if (track) {
        float heading = imu.getGestureHeading();
        float pitch = imu.getGesturePitch();
        float dPitch = (pitch - lastHeightPitch) / 50;
        float dHeading = angleDiff(heading, lastHeightHeading);
        driveAxis(dHeading, zPos);
        driveAxisOrientation(dPitch, rZ);
    } else {
        lastHeightHeading = imu.getGestureHeading();
        lastHeightPitch = imu.getGesturePitch();
    }
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

// Drive axis for updating position, currently only height
void GestureControl::driveAxis(float deltaDeg, float &pos) {
    float mag = fabsf(deltaDeg);
    float minZ = 0, maxZ = 800;

    // Checking for dead zone
    if (mag < deadZone)
        return;

    // Removing the dead-zone so the curve starts at zero
    mag -= deadZone;

    // Exponential Response
    float step = gain * powf(mag, exponent);
    float proposedPos = pos + copysignf(step, deltaDeg);

    // Applying boundary checks
    if (proposedPos > maxZ) { pos = maxRZ; }
    else if (proposedPos < minZ) { pos = minRZ; }
    else { pos += proposedPos; }
}

// drive axis for rotation
void GestureControl::driveAxisOrientation(float deltaDeg, float &pos) {
    // Calculate the proposed movement
    float mag = fabsf(deltaDeg);
    float step = gain * powf(mag, exponent);
    float proposedPos = pos + copysignf(step, deltaDeg);

    // Apply boundary checks
    if (proposedPos > maxRZ) { pos = maxRZ; }
    else if (proposedPos < minRZ) { pos = minRZ; }
    else { pos = proposedPos; }
}

// Keep heading differences inside −180 … +180 so wrap-around at 0/360 behaves.
float GestureControl::angleDiff(float a, float b) {
    float d = fmodf(a - b + 540.0f, 360.0f) - 180.0f;
    return d;
}

// For toggling the gripper, using two booleans
String GestureControl::toggleGripper(bool height, bool track) {
    static bool toggleState = false;
    String toggleStateString = "FALSE";
    static bool edgeTriggered = false;
    static unsigned long lastToggleTime = 0;
    const unsigned long debounceDelay = 600; // milliseconds

    if (height && track) {
        if (!edgeTriggered && millis() - lastToggleTime > debounceDelay) {
            toggleState = !toggleState;
            edgeTriggered = true;
            lastToggleTime = millis();
        }
    } else {
        edgeTriggered = false;
    }

    if (toggleState) {
        toggleStateString = "TRUE";
    } else {
        toggleStateString = "FALSE";
    }

    return toggleStateString;
}

// Drive axis for x and y position
float GestureControl::processAxisDelta(float delta, float currentValue, float maxValue) {
    // 1. Deadzone check
    float mag = fabsf(delta);
    if (mag < deadZone) return 0.0f;

    // 2. Remove deadzone and apply non-linear response
    mag -= deadZone;
    float step = gain * powf(mag, exponent);

    // 3. Apply direction
    step = copysignf(step, delta);

    // 4. Clamp to [-maxValue, maxValue]
    float available = (step > 0) ? (maxValue - currentValue) : (-maxValue - currentValue);
    return constrain(step, -fabsf(available), fabsf(available));
}
