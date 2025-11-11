#include <Arduino.h>
#include <Connections/MQTT.hpp>
#include <Connections/WiFiWPA2.hpp>
#include <Connections/IMU.hpp>
#include <Connections/FlexSensor.hpp>
#include "Control/GestureControl.hpp"
#include "Connections/UserButton.hpp"
#include <iostream>

// Creating WI-FI class
WiFiWPA2 wifi;

// Creating IMU class
IMU bno_IMU;

// Creating MQTT class connecting to NTNU Broker
MQTT mqtt(wifi.getWiFiClient(), "129.241.30.177", 1883);
String updateBoolValue(bool updating_);


// Defining MQTT Topics
Topic xLinear("cmd_vel", 100);
Topic cmd_vel("cmd_vel", 100);
Topic zAngular("cmd_vel", 100);
/*Topic rzValue("mom/rotValue", 100);
Topic xValue("mom/xValue", 100);
Topic yValue("mom/yValue", 100);
Topic zValue("mom/zValue", 100);
Topic gripper("mom/gripper", 100);
Topic invalidPose("mom/invalidPose", 100);
Topic updating("mom/updating", 100);

// joints
Topic xPos("mom/xActualValue", 100);
Topic yPos("mom/yActualValue", 100);
Topic zPos("mom/zActualValue", 100);
Topic joint5("mom/joint5", 100);
*/

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

//MQTT mqtt(wifi.getWiFiClient(), "10.24.8.108", 1883);
void setup() {
    Serial.begin(115200);

    // Initializing classes
    wifi.init();
    mqtt.init();
    bno_IMU.init();
    toggleButton.init();
    toggleHeight.init();
    toggleTrack.init();


    //mqtt.subscribe(xLinear);
    //mqtt.subscribe(zAngular);
    /* MQTT Subscribing to topic
    mqtt.subscribe(invalidPose);
    mqtt.subscribe(xPos);
    mqtt.subscribe(yPos);
    mqtt.subscribe(zPos);
    mqtt.subscribe(joint5);
    */
    // LED initialization
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
}

void loop() {
    // Call poll() regularly to allow the library to send MQTT keep alive which
    // avoids being disconnected by the broker
    mqtt.getMqttClient()->poll();
    Serial.println(cmd_vel.getTopic());


    if (toggleButton.toggle()) {
        // Running imu and gesture control, only if button is toggled
        bno_IMU.run();
        gestureControl.virtualJoystick(toggleTrack.read() && !toggleHeight.read());
        gestureControl.heightControl(toggleHeight.read() && !toggleTrack.read());
        toggleGripper = gestureControl.toggleGripper(toggleTrack.read(), toggleHeight.read());

        //
        gestureControl.updateOnStop(toggleHeight.read() or toggleTrack.read(), mqtt);

        updatingValue = updateBoolValue(toggleTrack.read() or toggleHeight.read());

        // Turning on green light
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
    } else {
        // Turning on red light
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, HIGH);

    }

    mqtt.publishJSON(cmd_vel, gestureControl.getX(), gestureControl.getY());
    //Serial.println(gestureControl.getX());

    /* Publishing position values
    mqtt.publish(xValue, gestureControl.getX());
    mqtt.publish(yValue, gestureControl.getY());
    mqtt.publish(zValue, gestureControl.getZ());
    mqtt.publish(rzValue, gestureControl.getRZ());
    mqtt.publishString(updating, updatingValue);

    // Gripper
    mqtt.publishString(gripper, toggleGripper);
    */
}

String updateBoolValue(bool updating_) {
    String string;
    if (updating_) {
        string = "TRUE";
    } else {
        string = "FALSE";
    }
    return string;
}