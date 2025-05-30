//
// Created by Herman Hårstad Gran on 03/04/2025.
//

#ifndef MOM_GLOVE_TOPIC_HPP
#define MOM_GLOVE_TOPIC_HPP

#include <Arduino.h>

/**
 * Topic class for creating topics to be published
 */
class Topic{
public:
    /**
     * Constructor for class
     * @param topic Topic you want to publish
     * @param interval Interval to publish the topic
     */
    Topic(const char* topic, float interval);

    /**
     * Getter function for last millis
     * @return Returning lastMillis
     */
    unsigned long getLastMillis();

    /**
     * Setter function for last millis
     * @param lastMillis The millis you want to assign
     */
    void setLastMillis(unsigned long lastMillis);

    /**
     * Getter function for topic
     * @return Returning Topic
     */
    const char* getTopic();

    /**
     * Getter function for interval
     * @return Returns Interval
     */
    float getInterval();

    /**
     * Used for updating last value sent
     * @param value last value sent
     */
    void updateLastValue(double value);

    /**
     * Used for checking of the last value sent is the same, in that case, not sending value to broker
     * @param value value to send
     * @return returning boolean if last value is different, returns true
     */
    bool update(double value);

private:
    unsigned long lastMillis;
    float interval;
    const char* topic;
    double lastValue;
};

#endif //MOM_GLOVE_TOPIC_HPP
