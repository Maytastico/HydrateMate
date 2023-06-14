
#include "HydratePresence.hpp"

HydratePresence::HydratePresence(uint8_t movement_sensor_pin){
    this->movement_sensor_pin = movement_sensor_pin;
}

/**
 * @brief Initializes the pins
 *
 */
void HydratePresence::begin(){
  pinMode(this->movement_sensor_pin, INPUT);
}

/**
 * @brief Checks if the person is moving
 *
 * @return true
 * @return false
 */
boolean HydratePresence::is_person_moving(){
    uint8_t sensor_value = this->read_sensor_status();
    this->store_messurement(sensor_value);
    
    if(sensor_value>0)
        SerialLogger::p_serial_logger->log(F("Person is moving"), Debugger::INFO);
        
    return sensor_value;
}

/**
 * @brief Reads the value from the sensor and evaluates the moisture level.
 * @return MoistureLevel
 *
 */
uint8_t HydratePresence::read_sensor_status() const{
  SerialLogger::p_serial_logger->log(F("Reading Movement Sensor"), Debugger::DEBUG);
  return digitalRead(this->movement_sensor_pin);
}

/**
 * @brief Returns the time of the last measurement
 *
 * @return uint32_t
 */
uint32_t HydratePresence::get_time_of_last_masurement(){
  return this->measurement.measured;
}

/**
 * @brief Stores the measurement
 *
 * @param movement_sensor_value
 */
void HydratePresence::store_messurement(uint8_t movement_sensor_value){
    this->measurement.movement_sensor = movement_sensor_value;
    this->measurement.measured = millis();
}
