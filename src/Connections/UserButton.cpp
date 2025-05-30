//
// Created by Herman Hårstad Gran on 08/04/2025.
//

#include "Connections/UserButton.hpp"

UserButton::UserButton(int pin_) : pin(pin_) {}

void UserButton::init() {
    pinMode(pin, INPUT_PULLUP);
}

bool UserButton::toggle() {

    bool buttonRead = !digitalRead(pin);
    if ((buttonRead and not toggled) && (millis() >= lastPressed + debounce )) {
        lastPressed = millis();
        toggled = true;
    } else if ((buttonRead and toggled) && (millis() >= lastPressed + debounce)) {
        lastPressed = millis();
        toggled = false;
    }

    return toggled;
}