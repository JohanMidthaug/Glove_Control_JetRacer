//
// Created by Herman Hårstad Gran on 29/04/2025.
//
#include "Control/GestureControl.hpp"
#include <cmath>

// Constructor
GestureControl::GestureControl(IMU& imu_):
imu(imu_),
minX(-800),
minY(-800),
minZ(-800),
maxX(800),
maxY(800),
maxZ(800),
interval(100) {}

// Init function, used in setup
void GestureControl::init() {}

// Virtual joystick function
void GestureControl::virtualJoystick(bool track) {
    if (track) {
        float roll = imu.getGestureRoll();
        float pitch = imu.getGesturePitch();
        float heading = imu.getGestureHeading();

        // Signed deltas from the reference we stored earlier
        float dRoll    = roll    - lastRoll;
        float dPitch   = pitch   - lastPitch;
        float dHeading = angleDiff(heading, lastHeading);

        const float cos45 = 0.7071;  // cos(45°)
        const float sin45 = 0.7071;  // sin(45°)

        // Transform the deltas (assuming dPitch corresponds to Y and dRoll to X in world coordinates)
        float transformedX = dHeading * cos45 - dPitch * sin45;
        float transformedY = dHeading * sin45 + dPitch * cos45;

        driveAxis(transformedX, xPos);
        driveAxis(transformedY, yPos);

    } else {
        lastHeading = imu.getGestureHeading();
        lastPitch = imu.getGesturePitch();
        lastRoll = imu.getGestureRoll();
    }
}

void GestureControl::orientation(bool track) {
    if (track) {
        rX = imu.roll();
        rY = imu.pitch();
        rZ = imu.heading();
    }
}

void GestureControl::heightControl(bool track) {
    if (track) {
        float heading = imu.getGestureHeading();
        float pitch = imu.getGesturePitch();
        float dPitch = (pitch - lastHeightPitch) / 50;
        float dHeading = angleDiff(heading, lastHeightHeading);
        Serial.print("dPitch: ");
        Serial.print(dPitch);
        Serial.print("dPitch: ");
        Serial.println(rZ);
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

void GestureControl::driveAxisOrientation(float deltaDeg, float &pos) {
    // 1. Calculate the proposed movement
    float mag = fabsf(deltaDeg);
    float step = gain * powf(mag, exponent);
    float proposedPos = pos + copysignf(step, deltaDeg);

    // 2. Apply boundary checks
    if (proposedPos > maxRZ) {
        pos = maxRZ;  // Clamp to maximum
    }
    else if (proposedPos < minRZ) {
        pos = minRZ;  // Clamp to minimum
    }
    else {
        pos = proposedPos;  // Accept the change
    }
}

// Keep heading differences inside −180 … +180 so wrap-around at 0/360 behaves.
float GestureControl::angleDiff(float a, float b) {
    float d = fmodf(a - b + 540.0f, 360.0f) - 180.0f;
    return d;   // signed shortest-arc difference
}


bool GestureControl::toggleGripper(bool height, bool track) {
    static bool toggleState = false;
    static bool edgeTriggered = false;
    static unsigned long lastToggleTime = 0;
    const unsigned long debounceDelay = 500; // milliseconds

    if (height && track) {
        if (!edgeTriggered && millis() - lastToggleTime > debounceDelay) {
            toggleState = !toggleState;
            edgeTriggered = true;
            lastToggleTime = millis();
        }
    } else {
        edgeTriggered = false;
    }

    return toggleState;
}