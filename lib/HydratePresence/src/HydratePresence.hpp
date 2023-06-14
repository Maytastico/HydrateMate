#ifndef HYDRATE_PRESENCE_HPP
#define HYDRATE_PRESENCE_HPP

#include <Arduino.h>
#include <SerialLogger.hpp>

struct PresenceValue{
    boolean movement_sensor;
    uint32_t measured;
};

class HydratePresence {
public:
  HydratePresence(uint8_t movement_sensor_pin);
  void begin();
  bool is_person_moving();

  uint32_t get_time_of_last_masurement();
  PresenceValue measurement;

private:
  uint8_t movement_sensor_pin;
  void store_messurement(uint8_t movement_sensor_value);
  uint8_t read_sensor_status() const;
};

#endif  // HYDRATE_PRESENCE_HPP
