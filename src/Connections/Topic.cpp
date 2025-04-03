//
// Created by Herman Hårstad Gran on 03/04/2025.
//

#include "Connections/Topic.hpp"

Topic::Topic(const char *topic, float interval) : topic(topic), interval(interval), lastMillis(millis()) {}

float Topic::getInterval() {
    return interval;
}

void Topic::setLastMillis(unsigned long lastMillis_) {
    lastMillis = lastMillis_;
}

unsigned long Topic::getLastMillis() {
    return lastMillis;
}

const char* Topic::getTopic() {
    return topic;
}