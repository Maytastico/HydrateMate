#include "HydrateMonitor.hpp"

namespace hydrate_monitor
{
  String HydrateMonitor::moistureLevelToString(hydrate_monitor::MoistureLevel moisture_level){
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


HydrateMonitor::HydrateMonitor(
  uint8_t moisture_sensor_pin, 
  uint8_t moisture_sensor_control_pin){

  this->moisture_sensor_pin = moisture_sensor_pin;
  this->moisture_sensor_control_pin = moisture_sensor_control_pin;
}


void HydrateMonitor::begin(){
  pinMode(this->moisture_sensor_pin, INPUT);
  pinMode(this->moisture_sensor_control_pin, OUTPUT);
}

MoistureLevel HydrateMonitor::readMoistureLevel(){
  plant_value.moisture_value = this->readMoistureValue();
  plant_value.moisture_level = this->evaluteMoistureLevel(this->plant_value.moisture_value);

  return plant_value.moisture_level;
}

MoistureLevel HydrateMonitor::evaluteMoistureLevel(uint16_t moisture_value){
  MoistureLevel temp_level;
  boolean in_error = false;

  //Evaluate Errors
  if(moisture_value < MOISTURE_MIN){
    Serial.println(F("Moisture Sensor not connected or in air!"));
    temp_level = NULL_VALUE;
    in_error = true;
  }
  else if (moisture_value > MOISTURE_MAX)
  {
    Serial.println(F("Moisture Sensor returns unexpectatly value!"));
    temp_level = UNEXPECTED_VALUE;
    in_error = true;
  }

  if (in_error){
    return temp_level;
  }

  // Mapping the min and max value to moiture level states 
  uint8_t maped_value = map(moisture_value, MOISTURE_MIN, MOISTURE_MAX, 0, MOISTURE_LEVEL_STATES);
  temp_level = static_cast<MoistureLevel>(maped_value);
  
  // Debugging message
  Serial.print(F("Water Level: "));
  Serial.println(this->moistureLevelToString(temp_level));

  return temp_level;
}

uint16_t HydrateMonitor::readMoistureValue(){
  Serial.println(F("Reading Moisture Sensor"));

  digitalWrite(this->moisture_sensor_control_pin, HIGH);
  uint16_t moiture_value = analogRead(this->moisture_sensor_pin);
  digitalWrite(this->moisture_sensor_control_pin, LOW);

  Serial.print(F("Moisture Sensor: "));
  Serial.println(String(moiture_value));

  return moiture_value;
}

