#ifndef HYDRATE_PRESENCE_HPP
#define HYDRATE_PRESENCE_HPP

#include <Arduino.h>

struct PresenceValue{
    boolean movement_sensor;
    uint32_t measured;
};

class HydratePresence {
public:
  HydratePresence(uint8_t sensorPin);
  void begin();
  bool isPersonPresent();
  bool isPersonMoving();

private:
  uint8_t movement_sensor_pin;
  PresenceValue measurement;
};

#endif  // HYDRATE_PRESENCE_HPP
