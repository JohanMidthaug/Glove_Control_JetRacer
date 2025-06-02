//
// Created by Herman Hårstad Gran on 08/04/2025.
//

#ifndef MOM_GLOVE_IMU_HPP
#define MOM_GLOVE_IMU_HPP

#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "utility/imumaths.h"
#include <vector>
#include <cmath>
#include "Adafruit_LSM9DS1.h"

/*!
 @startuml
class IMU {
  +IMU()
  +init()
  +run()
  +heading(): float
  +pitch(): float
  +roll(): float
  +getGestureHeading(): float
  +getGesturePitch(): float
  +getGestureRoll(): float
  +getbno(): Adafruit_BNO055&
  -bno: Adafruit_BNO055
  -absHeading: float
  -absPitch: float
  -absRoll: float
  -gestureHeading: float
  -gesturePitch: float
  -gestureRoll: float
}
@enduml
 */

/**
 * Class for initializing the BNO055 IMU sensor, also retrieving sensor data
 */
class IMU{
public:
    /**
     * Constructor for the class, creating an instance of the bno sensor using the Adafruit package
     */
    IMU();

    /**
     * Function for initializing the class, used in setup
     */
    void init();

    /**
     * Function for running sensor, calculating quaternions
     */
    void run();

    /**
     * Getter function for heading
     * @return
     */
    float heading();

    /**
     * Getter function for Pitch
     * @return
     */
    float pitch();

    /**
     * Getter function for Roll
     * @return
     */
    float roll();

    /**
     * Getter function for specific heading used in gesture control class
     * @return
     */
    float getGestureHeading();

    /**
     * Getter function for specific pitch used in gesture control class
     * @return
     */
    float getGesturePitch();

    /**
     * Getter function for specific roll used in gesture control class
     * @return
     */
    float getGestureRoll();

    /**
     * Getter function for bno instance from the adafruit package
     * @return
     */
    Adafruit_BNO055& getbno();

private:
    Adafruit_BNO055 bno;
    Adafruit_LSM9DS1 lsm;

    float gyroBiasX = 0, gyroBiasY = 0, gyroBiasZ = 0;
    unsigned long lastUpdate = 0;

    float absHeading;
    float absPitch;
    float absRoll;

    float gestureHeading;
    float gesturePitch;
    float gestureRoll;
};

#endif //MOM_GLOVE_IMU_HPP
