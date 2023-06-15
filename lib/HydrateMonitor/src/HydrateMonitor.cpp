#include "HydrateMonitor.hpp"

namespace hydrate_monitor
{
  /**
   * @brief Converts MoistureLevel to String. For debugging purposes.
  */
  String moistureLevelToString(MoistureLevel moisture_level){
    switch (moisture_level)
    {
    case hydrate_monitor::NULL_VALUE:
      return F("In Air");
      break;

    case hydrate_monitor::DRY:
        return F("Dry!");
        break;
    
    case hydrate_monitor::SOME_WATER:
        return F("Some Water");
        break;
    
    case hydrate_monitor::OPTIMAL:
        return F("Optimal");
        break;

    case hydrate_monitor::TO_MOIST:
        return F("Too moist!");
        break;

    case hydrate_monitor::UNEXPECTED_VALUE:
        return F("Unexpected!");
        break;

    default:
        return F("Not defined");
        break;
    }
}
} // namespace hydrate_monitor

/**
 * 
 * @brief Constructor for HydrateMonitor
 * HydraMonitor is responsible for reading the moisture value from the sensor and evaluating the moisture level.
 * @param moisture_sensor_pin Pin for moisture sensor
 * @param moisture_sensor_control_pin Pin for moisture sensor control
 * 
*/
HydrateMonitor::HydrateMonitor(
  uint8_t moisture_sensor_pin, 
  uint8_t moisture_sensor_control_pin){

  this->moisture_sensor_pin = moisture_sensor_pin;
  this->moisture_sensor_control_pin = moisture_sensor_control_pin;
}

/**
 * @brief Initializes the pins
 * 
*/
void HydrateMonitor::begin(){
  pinMode(this->moisture_sensor_pin, INPUT);
  pinMode(this->moisture_sensor_control_pin, OUTPUT);
}

/**
 * @brief Reads the value from the sensor and evaluates the moisture level.                                                                            
 * @return MoistureLevel
 * 
*/
hydrate_monitor::MoistureLevel HydrateMonitor::readMoistureLevel(){ 
  plant_value.moisture_value = this->readMoistureValue();
  plant_value.moisture_level = this->evaluteMoistureLevel(this->plant_value.moisture_value);

  return plant_value.moisture_level;
}

/**
 * @brief Evaluates the moisture level based on the moisture value provided as a parameter.
 * The moisture value is mapped to the moisture level states. When the moisture value is below the minimum value, the moisture level is set to NULL_VALUE.
 * to determine whether the sensor is connected or not. When the moisture value is above the maximum value, the moisture level is set to UNEXPECTED_VALUE.
 * In addition, the moisture level is printed to the serial monitor.
 * @param moisture_value Moisture value of the sensor
 * @return MoistureLevel
 * 
*/
hydrate_monitor::MoistureLevel HydrateMonitor::evaluteMoistureLevel(uint16_t moisture_value){
  hydrate_monitor::MoistureLevel temp_level;
  boolean in_error = false;

  //Evaluate Errors
  if(moisture_value < MOISTURE_MIN){
    Debugger::log(F("Moisture Sensor not connected or in air!"), Debugger::ERROR);
    temp_level = hydrate_monitor::NULL_VALUE;
    in_error = true;
  }
  else if (moisture_value > MOISTURE_MAX)
  {
    Debugger::log(F("Moisture Sensor returns unexpectatly value!"), Debugger::ERROR);
    temp_level = hydrate_monitor::UNEXPECTED_VALUE;
    in_error = true;
  }

  if (in_error){
    return temp_level;
  }

  // Mapping the min and max value to moiture level states 
  uint8_t maped_value = map(moisture_value, MOISTURE_MIN, MOISTURE_MAX, 0, MOISTURE_LEVEL_STATES);
  temp_level = static_cast<hydrate_monitor::MoistureLevel>(maped_value);
  
  // Debugging message
  Debugger::log(F("Water Level: "),  Debugger::DEBUG);
  Debugger::log(hydrate_monitor::moistureLevelToString(temp_level), Debugger::DEBUG);
  return temp_level;
}

/**
 * @brief Reads the moisture value from the analog pin where the sensor is connected.
 * In addition, the control pin is set to HIGH to activate the sensor and then set to LOW when the reading is done.
 * @return Moisture value
 * 
*/
uint16_t HydrateMonitor::readMoistureValue(){
  Debugger::log(F("Reading Moisture Sensor"), Debugger::DEBUG);

  digitalWrite(this->moisture_sensor_control_pin, HIGH);
  uint16_t moiture_value = analogRead(this->moisture_sensor_pin);
  digitalWrite(this->moisture_sensor_control_pin, LOW);

  Debugger::log(F("Moisture Sensor: "), Debugger::DEBUG);
  Debugger::log(String(moiture_value), Debugger::DEBUG);

  return moiture_value;
}

