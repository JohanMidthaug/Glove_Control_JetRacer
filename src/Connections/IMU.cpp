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
}

double IMU::heading() {
    double heading = bno.getVector(Adafruit_BNO055::VECTOR_EULER).x();
    Serial.print("Heading: ");
    Serial.print(heading);
    return heading;
}

double IMU::pitch() {
    double pitch = bno.getVector(Adafruit_BNO055::VECTOR_EULER).y();
    Serial.print("Pitch: ");
    Serial.print(pitch);
    return pitch;
}

double IMU::roll() {
    double roll = bno.getVector(Adafruit_BNO055::VECTOR_EULER).z();
    Serial.print("Roll: ");
    Serial.print(roll);
    return roll;
}

std::vector<double> IMU::position() {
    std::vector<double> position;
    return position;
}

Adafruit_BNO055& IMU::getbno() {
    return bno;
}