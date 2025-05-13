//
// Created by Herman Hårstad Gran on 08/04/2025.
//
#include "Connections/IMU.hpp"

IMU::IMU() {
    bno = Adafruit_BNO055(55, 0x28);
}

void IMU::init() {
    if (!bno.begin()) {
        Serial.println("BNO055 not detected ... Check wiring or I2C address");
    }
    // Setting to not use external crystal oscillator
    bno.setExtCrystalUse(false);
    bno.setMode(OPERATION_MODE_NDOF);
}

#include <cmath>

void quaternionToEulerRad(const imu::Quaternion& q, float& roll, float& pitch, float& yaw) {
    // Assuming quaternion is normalized and in (w, x, y, z)
    float w = q.w();
    float x = q.x();
    float y = q.y();
    float z = q.z();

    // Roll (X-axis rotation)
    float sinr_cosp = 2.0f * (w * x + y * z);
    float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
    roll = std::atan2(sinr_cosp, cosr_cosp);

    // Pitch (Y-axis rotation)
    float sinp = 2.0f * (w * y - z * x);
    if (std::abs(sinp) >= 1.0f)
        pitch = std::copysign(M_PI / 2.0f, sinp); // Use 90 degrees if out of range
    else
        pitch = std::asin(sinp);

    // Yaw (Z-axis rotation)
    float siny_cosp = 2.0f * (w * z + x * y);
    float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    yaw = std::atan2(siny_cosp, cosy_cosp);
}

void IMU::run() {
    static bool first = true;

    static imu::Quaternion qPrev;

    if (first) { first = false; return; }

    imu::Quaternion qNow = bno.getQuat();

    float roll, pitch, yaw;
    quaternionToEulerRad(qNow, roll, pitch, yaw);

    absRoll = roll;
    absPitch = pitch;
    absHeading = yaw;

    // Hotfix for tracking issue
    imu::Quaternion dQ = qPrev.conjugate() * qNow;
    qPrev = qNow;

    float dRollRad = 2.0f * dQ.x();
    float dPitchRad = 2.0f * dQ.y();
    float dHeadingRad = 2.0f * dQ.z();

    float dRollDeg = dRollRad * 57.29578f;
    float dPitchDeg = dPitchRad * 57.29578f;
    float dHeadingDeg = dHeadingRad * 57.29578f;

    gestureRoll += dRollDeg;
    gesturePitch += dPitchDeg;
    gestureHeading += dHeadingDeg;
}

float IMU::heading() {
    return absHeading;
}

float IMU::pitch() {
    return absPitch;
}

float IMU::roll() {
    return absRoll;
}

float IMU::getGestureHeading() {
    return gestureHeading;
}

float IMU::getGesturePitch() {
    return gesturePitch;
}

float IMU::getGestureRoll() {
    return gestureRoll;
}

Adafruit_BNO055& IMU::getbno() {
    return bno;
}