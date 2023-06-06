
#include "HydratePresence.hpp"

HydratePresence::HydratePresence(uint8_t movement_sensor_pin){
    this->movement_sensor_pin = movement_sensor_pin;
}

void HydratePresence::begin(){
  pinMode(this->movement_sensor_pin, INPUT);
}

boolean HydratePresence::isPersonMoving(){
    uint8_t sensor_value = this->readMovementSensorValue();
    this->storeMeasurement(sensor_value);
    
    if(sensor_value>0)
        Serial.println(F("Person is moving"));
        
    return sensor_value;
}

uint8_t HydratePresence::readMovementSensorValue(){
  Serial.println(F("Reading Movement Sensor"));
  return digitalRead(this->movement_sensor_pin);
}

uint32_t HydratePresence::getTimeOfLastMeasurement(){
  return this->measurement.measured;
}

void HydratePresence::storeMeasurement(uint8_t movement_sensor_value){
    this->measurement.movement_sensor = movement_sensor_value;
    this->measurement.measured = millis();
}
