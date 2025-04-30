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

void IMU::run() {
    // Orientation tracking with quaternions (using delta pos for the total position)
    static bool first = true;
    static imu::Quaternion qPrev;

    imu::Quaternion qNow = bno.getQuat();

    if (first) { qPrev = qNow; first = false; return; }

    imu::Quaternion dQ = qPrev.conjugate() * qNow;
    qPrev = qNow;

    float dRollRad = 2.0f * dQ.x();
    float dPitchRad = 2.0f * dQ.y();
    float dHeadingRad = 2.0f * dQ.z();

    float dRollDeg = dRollRad * 57.29578f;
    float dPitchDeg = dPitchRad * 57.29578f;
    float dHeadingDeg = dHeadingRad * 57.29578f;

    absRoll += dRollDeg;
    absHeading += dHeadingDeg;
    absPitch += dPitchDeg;
}

double IMU::heading() {
    return absHeading;
}

double IMU::pitch() {
    return absPitch;
}

double IMU::roll() {
    return absRoll;
}

Adafruit_BNO055& IMU::getbno() {
    return bno;
}