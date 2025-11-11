//
// Created by Herman Hårstad Gran on 29/04/2025.
//
#include "Control/GestureControl.hpp"
#include <cmath>

// Constructor
GestureControl::GestureControl(IMU& imu_): imu(imu_), interval(100) {}

// Init function, used in setup
void GestureControl::init() {}

// Virtual joystick function, got a little help from GPT for creating polar coordinates update for IMU control.
// Makes it more intuitive
void GestureControl::virtualJoystick(bool track) {
    if (track) {
        float dHeading = lastHeading - imu.getGestureHeading();
        float dPitch =  lastPitch - imu.getGesturePitch();

        throttle = map(dHeading, -40, 40, -1, 1);
        steering = map(dPitch, -80, 80, -1, 1);

        //Serial.printf("Delte Heading: %f \n Delta Pitch: %f", throttle, steering);
    } else {
        throttle = 0;
        steering = 0;
        lastHeading = imu.getGestureHeading();
        lastPitch = imu.getGesturePitch();
        //Serial.printf("heading: %f, Pitch: %f\n", imu.getGestureHeading(), imu.getGesturePitch());
    }
}

// Orientation
void GestureControl::orientation(bool track) {
    if (track) {
        rZ = imu.heading();
    }
}

// Function for height control
void GestureControl::heightControl(bool track) {
    if (track) {
        float heading = imu.getGestureHeading();
        float pitch = imu.getGesturePitch();
        float dPitch = (pitch - lastHeightPitch) / 50;
        float dHeading = angleDiff(heading, lastHeightHeading);
        driveAxis(dHeading, zPos);
        driveAxisOrientation(dPitch, rZ);
    } else {
        lastHeightHeading = imu.getGestureHeading();
        lastHeightPitch = imu.getGesturePitch();
    }
}

float GestureControl::getX() const {
    return xPos;
}

float GestureControl::getY() const {
    return yPos;
}

float GestureControl::getZ() const {
    return zPos;
}

float GestureControl::getRZ() const {
    return rZ;
}

// Drive axis for updating position, currently only height
void GestureControl::driveAxis(float deltaDeg, float &pos) {
    float mag = fabsf(deltaDeg);
    float minZ = 0, maxZ = 800;

    // Checking for dead zone
    if (mag < deadZone)
        return;

    // Removing the dead-zone so the curve starts at zero
    mag -= deadZone;

    // Exponential Response
    float step = gain * powf(mag, 1.6);
    float proposedPos = pos + copysignf(step, deltaDeg);

    // Applying boundary checks
    if (proposedPos > maxZ) { pos = maxRZ; }
    else if (proposedPos < minZ) { pos = minRZ; }
    else { pos = proposedPos; }
}

// drive axis for rotation
void GestureControl::driveAxisOrientation(float deltaDeg, float &pos) {
    // Calculate the proposed movement
    float mag = fabsf(deltaDeg);
    float step = gain * powf(mag, exponent);
    float proposedPos = pos + copysignf(step, deltaDeg);

    // Apply boundary checks
    if (proposedPos > maxRZ) { pos = maxRZ; }
    else if (proposedPos < minRZ) { pos = minRZ; }
    else { pos = proposedPos; }
}

// Keep heading differences inside −180 … +180 so wrap-around at 0/360 behaves.
float GestureControl::angleDiff(float a, float b) {
    float d = fmodf(a - b + 540.0f, 360.0f) - 180.0f;
    return d;
}

// For toggling the gripper, using two booleans
String GestureControl::toggleGripper(bool height, bool track) {
    static bool toggleState = false;
    String toggleStateString = "FALSE";
    static bool edgeTriggered = false;
    static unsigned long lastToggleTime = 0;
    const unsigned long debounceDelay = 600; // milliseconds

    if (height && track) {
        if (!edgeTriggered && millis() - lastToggleTime > debounceDelay) {
            toggleState = !toggleState;
            edgeTriggered = true;
            lastToggleTime = millis();
        }
    } else {
        edgeTriggered = false;
    }

    if (toggleState) {
        toggleStateString = "TRUE";
    } else {
        toggleStateString = "FALSE";
    }

    return toggleStateString;
}

// Drive axis for x and y position
float GestureControl::processAxisDelta(float delta, float currentValue, float maxValue) {
    // 1. Deadzone check
    float mag = fabsf(delta);
    if (mag < deadZone) return 0.0f;

    // 2. Remove deadzone and apply non-linear response
    mag -= deadZone;
    float step = gain * powf(mag, exponent);

    // 3. Apply direction
    step = copysignf(step, delta);

    // 4. Clamp to [-maxValue, maxValue]
    float available = (step > 0) ? (maxValue - currentValue) : (-maxValue - currentValue);
    return constrain(step, -fabsf(available), fabsf(available));
}

void GestureControl::setX(float pos) {
    xPos = pos;
}

void GestureControl::setY(float pos) {
    yPos = pos;
}

void GestureControl::setZ(float pos) {
    zPos = pos;
}

void GestureControl::computeForwardKinematics(const double q[6], bool trigger) {
    if (!trigger && prevTrigger) {
        Serial.println("Calculating");

        // 3) UR5e MDH parameters.  Note carefully the “index shift”:
        //    a_prev[i] and alpha_prev[i] correspond to the (i+0) link’s parameters,
        //    while d[i] corresponds to link i+1’s offset in Z.
        //
        static const double a_prev[6]     = {  0.0,    0.0,   -0.425,  -0.3922,  0.0,    0.0    };
        static const double alpha_prev[6] = {  0.0,  -M_PI/2, 0.0,    0.0,     M_PI/2, -M_PI/2 };
        static const double d_vals[6]     = {  0.1625, 0.0,    0.0,    0.1333,  0.0997,  0.0996 };

        // Helper to form a single MDH 4×4 from (a_prev, alpha_prev, q_i, d_i)
        auto makeMDH = [&](int i, double T_out[4][4]) {
            // i goes from 0..5, corresponding to link #i+1 in the chain.
            double c_alpha = cos(alpha_prev[i]);
            double s_alpha = sin(alpha_prev[i]);
            double c_theta = cos(q[i]);
            double s_theta = sin(q[i]);
            double a_i      = a_prev[i];
            double d_i      = d_vals[i];

            // The “Modified DH” row‐by‐row (see standard robotics texts):
            //   T_i = RotX(alpha_{i-1}) · TransX(a_{i-1}) · RotZ(theta_i) · TransZ(d_i)
            // Expanding that gives (rows in homogeneous form):
            //
            //  [  cosθ_i,  –sinθ_i·cosα_{i-1},   sinθ_i·sinα_{i-1},   a_{i-1}·cosθ_i  ]
            //  [  sinθ_i,   cosθ_i·cosα_{i-1},  –cosθ_i·sinα_{i-1},   a_{i-1}·sinθ_i  ]
            //  [   0,            sinα_{i-1},         cosα_{i-1},         d_i       ]
            //  [   0,              0,                 0,               1          ]
            //
            T_out[0][0] =  c_theta;
            T_out[0][1] = -s_theta * c_alpha;
            T_out[0][2] =  s_theta * s_alpha;
            T_out[0][3] =  a_i * c_theta;

            T_out[1][0] =  s_theta;
            T_out[1][1] =  c_theta * c_alpha;
            T_out[1][2] = -c_theta * s_alpha;
            T_out[1][3] =  a_i * s_theta;

            T_out[2][0] =  0.0;
            T_out[2][1] =  s_alpha;
            T_out[2][2] =  c_alpha;
            T_out[2][3] =  d_i;

            T_out[3][0] =  0.0;
            T_out[3][1] =  0.0;
            T_out[3][2] =  0.0;
            T_out[3][3] =  1.0;
        };

        // 4) Build T₀…T₅
        double T_i[6][4][4];
        for (int i = 0; i < 6; i++) {
            makeMDH(i, T_i[i]);
        }

        // 5) Multiply them:  Tfinal = T₀ · T₁ · T₂ · T₃ · T₄ · T₅
        auto matMul4 = [&](const double A[4][4], const double B[4][4], double C[4][4]) {
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    double sum = 0.0;
                    for (int k = 0; k < 4; k++) {
                        sum += A[r][k] * B[k][c];
                    }
                    C[r][c] = sum;
                }
            }
        };

        double Tfinal[4][4], Ttmp[4][4];
        // Copy Tfinal = T₀
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                Tfinal[r][c] = T_i[0][r][c];
            }
        }
        // Now do Tfinal = Tfinal * T₁, then (… * T₂), … up to T₅
        for (int k = 1; k < 6; k++) {
            matMul4(Tfinal, T_i[k], Ttmp);
            // Copy Ttmp → Tfinal
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    Tfinal[r][c] = Ttmp[r][c];
                }
            }
        }

        // 6) Extract (x, y, z) **in meters**, then multiply by 1000 → millimeters
        double x_m = Tfinal[0][3];
        double y_m = Tfinal[1][3];
        double z_m = Tfinal[2][3];

        xPos = x_m * 1000.0;
        yPos = y_m * 1000.0;
        zPos = z_m * 1000.0;

        Serial.print(  "✅ FK result (mm): X = ");
        Serial.print(xPos,  1);  // e.g. “250.0”
        Serial.print("  Y = ");
        Serial.print(yPos,  1);
        Serial.print("  Z = ");
        Serial.println(zPos, 1);
    }

    // 7) Always update prevTrigger for the next call
    prevTrigger = trigger;
}

float GestureControl::map(float x, float in_min, float in_max, float out_min, float out_max) {
    // Guard against division by zero
    float in_range = in_max - in_min;
    if (in_range == 0.0f) {
        // If input range is zero, return the lower bound of the output range
        return out_min;
    }

    // Normalize x to [0, 1] within the input range, then scale to output range
    float t = (x - in_min) / in_range;
    float out = out_min + t * (out_max - out_min);

    // Clamp to the output range (handles reversed ranges as well)
    float minO = fminf(out_min, out_max);
    float maxO = fmaxf(out_min, out_max);
    if (out < minO) return minO;
    if (out > maxO) return maxO;
    return out;
}

float GestureControl::getThrottle() const {
    return throttle;
}

float GestureControl::getSteering() const {
    return steering;
}