#include <Arduino.h>
#include <Connections/MQTT.hpp>
#include <Connections/WiFiWPA2.hpp>
#include <Connections/IMU.hpp>
#include <Connections/FlexSensor.hpp>
#include "Control/GestureControl.hpp"
#include "Connections/UserButton.hpp"

// Creating WI-FI class
WiFiWPA2 wifi;

// Creating IMU class
IMU bno_IMU;

// Creating MQTT class connecting to NTNU Broker
//MQTT mqtt(wifi.getWiFiClient(), "129.241.30.177", 1883);
String updateBoolValue(bool updating_);


// Defining MQTT Topics
Topic rzValue("mom/rotValue", 100);
Topic xValue("mom/xValue", 100);
Topic yValue("mom/yValue", 100);
Topic zValue("mom/zValue", 100);
Topic gripper("mom/gripper", 100);
Topic invalidPose("mom/invalidPose", 100);
Topic updating("mom/updating", 100);

// joints
Topic joint0("mom/joint0", 100);
Topic joint1("mom/joint1", 100);
Topic joint2("mom/joint2", 100);
Topic joint3("mom/joint3", 100);
Topic joint4("mom/joint4", 100);
Topic joint5("mom/joint5", 100);

// Defining flex sensors
FlexSensor toggleHeight(4, 3150);
FlexSensor toggleTrack(7, 3150);

// Gesture Control
GestureControl gestureControl(bno_IMU);

// User button
UserButton toggleButton(10);

// Value for sending gripper
String toggleGripper = "FALSE";
String updatingValue = "FALSE";

MQTT mqtt(wifi.getWiFiClient(), "10.24.8.108", 1883, (toggleHeight.read() or toggleTrack.read()), gestureControl);
void setup() {
    Serial.begin(115200);

    // Initializing classes
    wifi.init();
    mqtt.init();
    bno_IMU.init();
    toggleButton.init();
    toggleHeight.init();
    toggleTrack.init();

    // MQTT Subscribing to topic
    mqtt.subscribe(invalidPose);
    mqtt.subscribe(joint0);
    mqtt.subscribe(joint1);
    mqtt.subscribe(joint2);
    mqtt.subscribe(joint3);
    mqtt.subscribe(joint4);
    mqtt.subscribe(joint5);


    // LED initialization
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
}

void loop() {
    // Call poll() regularly to allow the library to send MQTT keep alive which
    // avoids being disconnected by the broker
    mqtt.getMqttClient()->poll();

    if (toggleButton.toggle()) {
        // Running imu and gesture control, only if button is toggled
        bno_IMU.run();
        gestureControl.virtualJoystick(toggleTrack.read() && !toggleHeight.read());
        gestureControl.heightControl(toggleHeight.read() && !toggleTrack.read());
        toggleGripper = gestureControl.toggleGripper(toggleTrack.read(), toggleHeight.read());

        //gestureControl.computeForwardKinematics(mqtt.getJointAngles(), toggleTrack.read() or toggleHeight.read());

        // Turning on green light
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
    } else {
        // Turning on red light
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, HIGH);

    }

    // Publishing position values
    mqtt.publish(xValue, gestureControl.getX());
    mqtt.publish(yValue, gestureControl.getY());
    mqtt.publish(zValue, gestureControl.getZ());

    // Orientation value
    mqtt.publish(rzValue, gestureControl.getRZ());

    // Gripper
    mqtt.publishString(gripper, toggleGripper);
}
