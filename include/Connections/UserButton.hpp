//
// Created by Herman Hårstad Gran on 08/04/2025.
//

#ifndef MOM_GLOVE_USERBUTTON_HPP
#define MOM_GLOVE_USERBUTTON_HPP

#include "Arduino.h"

/**
 * Class for initializing pushbutton and some function for reading
 */
class UserButton{
public:
    /**
     * Constructor function, internal pull up
     * @param pin_ input pin number
     */
    UserButton(int pin_);

    /**
     * Init function, used in setup
     */
    void init();

    /**
     * Toggle function when input pin pulled low
     * @return returning true or false in a toggle fashion
     */
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
