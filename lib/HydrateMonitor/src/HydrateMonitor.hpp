#ifndef HYDRATEMONITOR_HPP
#define HYDRATEMONITOR_HPP

#include <Arduino.h>

#define MOISTURE_MAX 255
#define MOISTURE_MIN 1


#define MOISTURE_LEVEL_STATES 5

namespace hydrate_monitor{

  enum MoistureLevel{
      NULL_VALUE,
      UNEXPECTED_VALUE,
      DRY,
      SOME_WATER,
      OPTIMAL,
      TO_MOIST,
  };

  struct PlantValue{
    uint16_t moisture_value;
    MoistureLevel moisture_level;
  };

  String moistureLevelToString(MoistureLevel moisture_level);
}
class HydrateMonitor {
public:
  HydrateMonitor(uint8_t moisture_sensor_pin, uint8_t moisture_sensor_control_pin); // Konstruktor hinzugefügt
  void begin();
  hydrate_monitor::MoistureLevel readMoistureLevel();
  
  hydrate_monitor::PlantValue plant_value;

private:
  uint8_t moisture_sensor_pin; // SoftwareSerial-Instanz hinzugefügt
  uint8_t moisture_sensor_control_pin; // SoftwareSerial-Instanz hinzugefügn

  hydrate_monitor::evaluteMoistureLevel(uint16_t moisture_value);
  uint16_t readMoistureValue();
};

#endif  // HYDRATEMONITOR_HPP

