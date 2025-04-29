//
// Created by Herman Hårstad Gran on 29/04/2025.
//

#ifndef MOM_GLOVE_FLEXSENSOR_HPP
#define MOM_GLOVE_FLEXSENSOR_HPP

#include "Arduino.h"

/**
 * Class for flex sensors, reading and returning if activated above a certain threshold.
 * Can also use for speed (according to how much you flex the sensor.
 */
class FlexSensor{
public:
    /**
     * Initializing instance of the class
     * @param inputPin_ Defining input pin
     * @param threshold_ Define threshold value for bool
     */
    FlexSensor(int inputPin_, int threshold_);

    void init();

    /**
     * Returning true or false for flex above threshold
     * @return
     */
    bool read();

private:
    int inputPin;
    int threshold;
};

#endif //MOM_GLOVE_FLEXSENSOR_HPP
