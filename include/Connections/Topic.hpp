//
// Created by Herman Hårstad Gran on 03/04/2025.
//

#ifndef MOM_GLOVE_TOPIC_HPP
#define MOM_GLOVE_TOPIC_HPP

#include <Arduino.h>

class Topic{
public:
    Topic(const char* topic, float interval);

    unsigned long getLastMillis();

    void setLastMillis(unsigned long lastMillis);

    const char* getTopic();

    float getInterval();

private:
    unsigned long lastMillis;
    float interval;
    const char* topic;
};

#endif //MOM_GLOVE_TOPIC_HPP
