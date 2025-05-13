//
// Created by Herman Hårstad Gran on 08/04/2025.
//

#include "Connections/UserButton.hpp"

UserButton::UserButton(int pin_) : pin(pin_) {}

void UserButton::init() {
    pinMode(pin, INPUT_PULLUP);
    pinMode(LED_GREEN, OUTPUT);
}


bool UserButton::buttonToggle() {

    bool read = !digitalRead(pin);

    if ((read and not toggled) && (millis() >= lastPressed + debounce )) {
        lastPressed = millis();
        toggled = true;
        digitalWrite(LED_GREEN, HIGH);
    } else if ((read and toggled) && (millis() >= lastPressed + debounce)) {
        lastPressed = millis();
        toggled = false;
        digitalWrite(LED_GREEN, LOW);
    }

    return toggled;
}