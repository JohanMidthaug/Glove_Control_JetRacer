#include "Connections/IMU.hpp"

IMU::IMU() : lsm() {
    absHeading = absPitch = absRoll = 0;
    gestureHeading = gesturePitch = gestureRoll = 0;
}

void IMU::init() {
    if (!lsm.begin()) {
        Serial.println("LSM9DS1 not detected ... Check wiring or I2C address");
        while (1);
    }

    lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
    lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
    lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
}

void IMU::run() {
    sensors_event_t a, m, g, temp;
    lsm.getEvent(&a, &m, &g, &temp);

    // --- Orientation from accelerometer ---
    float ax = a.acceleration.x;
    float ay = a.acceleration.y;
    float az = a.acceleration.z;

    absRoll = atan2(ay, az) * 180.0 / M_PI;
    absPitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / M_PI;

    // --- Heading from magnetometer ---
    float mx = m.magnetic.x;
    float my = m.magnetic.y;

    absHeading = atan2(my, mx) * 180.0 / M_PI;
    if (absHeading < 0) absHeading += 360.0;

    // --- Accurate dt (in seconds) ---
    unsigned long now = millis();
    float dt = (now - lastUpdate) / 1000.0f;
    lastUpdate = now;

    // --- Gyroscope-based delta integration (bias-corrected) ---
    float dRollDeg = (g.gyro.x - gyroBiasX) * dt * 180.0 / M_PI;
    float dPitchDeg = (g.gyro.y - gyroBiasY) * dt * 180.0 / M_PI;
    float dHeadingDeg = (g.gyro.z - gyroBiasZ) * dt * 180.0 / M_PI;

    gestureRoll    += dRollDeg;
    gesturePitch   += dPitchDeg;
    gestureHeading += dHeadingDeg;
}

float IMU::heading() {
    return absHeading;
}

float IMU::pitch() {
    return absPitch;
}

float IMU::roll() {
    return absRoll;
}

float IMU::getGestureHeading() {
    return gestureHeading;
}

float IMU::getGesturePitch() {
    return gesturePitch;
}

float IMU::getGestureRoll() {
    return gestureRoll;
}
