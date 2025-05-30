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

    bool toggle();

private:
    int pin;
    double debounce = 500;
    unsigned long lastPressed = 0;
    bool toggled = false;
    bool risingEdgeBool;
    bool risingEdgeBool0;
};

#endif //MOM_GLOVE_USERBUTTON_HPP
