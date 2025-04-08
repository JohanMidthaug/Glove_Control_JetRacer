//
// Created by Herman Hårstad Gran on 08/04/2025.
//

#ifndef MOM_GLOVE_USERBUTTON_HPP
#define MOM_GLOVE_USERBUTTON_HPP

#include "Arduino.h"

class UserButton{
public:
    UserButton(int pin_);

    void init();

    float sensorRead();

    bool buttonRead();

private:
    int pin;

};

#endif //MOM_GLOVE_USERBUTTON_HPP
