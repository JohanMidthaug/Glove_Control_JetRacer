//
// Created by Herman Hårstad Gran on 29/04/2025.
//
#include "Connections/FlexSensor.hpp"

FlexSensor::FlexSensor(int inputPin_, int threshold_) : inputPin(inputPin_), threshold(threshold_) {}

void FlexSensor::init() {
    pinMode(inputPin, INPUT);
}

bool FlexSensor::read() {
    Serial.println(analogRead(inputPin));
    if (analogRead(inputPin) > threshold) {
        return true;
    } else {
        return false;
    }
}