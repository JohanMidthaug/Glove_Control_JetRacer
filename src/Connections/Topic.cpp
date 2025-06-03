//
// Created by Herman Hårstad Gran on 03/04/2025.
//

#include "Connections/Topic.hpp"

// Constructor
Topic::Topic(const char *topic, float interval) : topic(topic), interval(interval), lastMillis(millis()), lastValue(0) {}

// Getter function for update interval
float Topic::getInterval() {
    return interval;
}

// Updating last update time
void Topic::setLastMillis(unsigned long lastMillis_) {
    lastMillis = lastMillis_;
}

// Getter function for last update time
unsigned long Topic::getLastMillis() {
    return lastMillis;
}

// Getter function for topic
const char* Topic::getTopic() {
    return topic;
}

// Updating last value sent to broker
void Topic::updateLastValue(double value) {
    lastValue = value;
}

// Checking of last value sent to broker os different from current
bool Topic::update(double value) {
    if (lastValue != value) {return true;} else {return false;}
}

// Updating last sent string
void Topic::updateLastString(String string) {
    lastString = string;
}

// Checking of the last sent bool is the same as this one
bool Topic::updateString(String string) {
    if (lastString != string) {return true;} else {return false;}
}
