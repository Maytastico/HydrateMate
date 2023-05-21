#ifndef HYDRATE_PRESENCE_HPP
#define HYDRATE_PRESENCE_HPP

#include <Arduino.h>

struct PresenceValue{
    boolean movement_sensor;
    uint32_t measured;
};

class HydratePresence {
public:
  HydratePresence(uint8_t movement_sensor_pin);
  void begin();
  bool isPersonMoving();
  uint32_t getTimeOfLastMeasurement();

private:
  uint8_t movement_sensor_pin;
  PresenceValue measurement;

  void storeMeasurement(uint8_t movement_sensor_value);
  uint8_t readMovementSensorValue();
};

#endif  // HYDRATE_PRESENCE_HPP
