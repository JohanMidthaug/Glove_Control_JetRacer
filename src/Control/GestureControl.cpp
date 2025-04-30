//
// Created by Herman Hårstad Gran on 29/04/2025.
//
#include "Control/GestureControl.hpp"

GestureControl::GestureControl(IMU& imu_):
imu(imu_),
minX(-850),
minY(-850),
minZ(-850),
maxX(850),
maxY(850),
maxZ(850),
xPos(100),
yPos(100),
zPos(100),
lastHeading(0),
lastPitch(0),
lastRoll(0),
interval(100),
lastUpdate(0) {}

void GestureControl::init() {}

void GestureControl::track(FlexSensor& flexSensor) {
    if (flexSensor.read()) {

        if (millis() > lastUpdate + interval) {
            lastUpdate = millis();
            double roll = imu.roll();
            double pitch = imu.pitch();
            double heading = imu.heading();

            if (roll > lastRoll) {
                xPos += roll/lastRoll + 1; // Må endre
            } else {
                xPos -= roll/lastRoll + 1; // Må endre
            }

            if (pitch > lastPitch) {
                yPos += pitch/lastPitch + 1; // Må endre
            } else {
                yPos -= pitch/lastPitch + 1; // Må endre
            }

            if (heading > lastHeading) {
                zPos += heading/lastHeading + 1; // Må endre
            } else {
                zPos += heading/lastHeading + 1; // Må endre
            }
        }

    } else {
        lastHeading = imu.heading();
        lastPitch = imu.pitch();
        lastRoll = imu.roll();
    }
}

float GestureControl::getX() {
    return xPos;
}

float GestureControl::getY() {
    return yPos;
}

float GestureControl::getZ() {
    return zPos;
}

