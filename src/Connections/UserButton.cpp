//
// Created by Herman Hårstad Gran on 08/04/2025.
//

#include "Connections/UserButton.hpp"

UserButton::UserButton(int pin_) : pin(pin_) {}

void UserButton::init() {
    pinMode(pin, INPUT);
}

float UserButton::sensorRead() {

}

bool UserButton::buttonRead() {

}