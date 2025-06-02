//
// Created by Herman Hårstad Gran on 03/04/2025.
//

#ifndef MOM_GLOVE_TOPIC_HPP
#define MOM_GLOVE_TOPIC_HPP

#include <Arduino.h>

/*!
 * @startuml
 * class Topic {
 *   +Topic(topic: const char*, interval: float)
 *   +getLastMillis(): unsigned long
 *   +setLastMillis(lastMillis: unsigned long)
 *   +getTopic(): const char*
 *   +getInterval(): float
 *   +updateLastValue(value: double)
 *   +update(value: double): bool
 *   -lastMillis: unsigned long
 *   -interval: float
 *   -topic: const char*
 *   -lastValue: double
}
@enduml
 */

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
     * Function for updating the last string sent
     * @param string THe string value
     */
    void updateLastString(String string);

    /**
     * Function for checking of the current string to send is the same as before
     * @param string String to send
     * @return Returns true or false based on if the current value is the same as the old
     */
    bool updateString(String string);

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
    String lastString;
};

#endif //MOM_GLOVE_TOPIC_HPP
