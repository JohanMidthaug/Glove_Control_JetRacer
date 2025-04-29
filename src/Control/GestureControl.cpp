//
// Created by Herman Hårstad Gran on 29/04/2025.
//
#include "Control/GestureControl.hpp"

GestureControl::GestureControl(IMU& imu_) :imu(imu_), minX(-850), minY(-850), minZ(-850), maxX(850), maxY(850), maxZ(850), x(100), y(100), z(100) {}

void GestureControl::init() {}

void GestureControl::track(FlexSensor& flexSensor) {
    if (flexSensor.read()) {

    } else {
        lastHeading = imu.heading();
        lastPitch = imu.pitch();
        lastRoll = imu.roll();
    }
}

float GestureControl::getx() {
    return x;
}

float GestureControl::gety() {
    return y;
}

float GestureControl::getz() {
    return z;
}

