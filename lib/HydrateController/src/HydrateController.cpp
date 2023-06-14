#include <HydrateController.hpp>

namespace PlantStates
{

/**
 * @brief Convert MonitoringStates to String
*/
  String state_to_string(MonitoringStates state){
    switch (state) {
      case INIT:
        return F("INIT");
        break;
      case SLEEP:
        return F("SLEEP");
        break;
      case MONITORING:
        return F("MONITORING");
        break;
      case PRESENCE:
        return F("PRESENCE");
        break;
      case NOTIFING:
        return F("NOTIFING");
        break;
    }
    return F("UNKNOWN");
  }
} // namespace PlantStates

HydrateController *HydrateController::p_hydration_controller = nullptr;

void HydrateController::serialHandler(int argCnt, char **args){
  if(p_hydration_controller != nullptr){
    if(strcmp(args[1], "s") == 0){
        p_hydration_controller->print_state();
    }
    else if (strcmp(args[1], "r") == 0)
    {
        p_hydration_controller->p_hydration_monitor->readMoistureLevel();
        p_hydration_controller->print_state();
    }
    else if (strcmp(args[1], "presence") == 0)
    {   
        p_hydration_controller->set_state(PlantStates::PRESENCE);
        Serial.println(F("Presence"));
    }
    else if (strcmp(args[1], "init") == 0)
    {   
        p_hydration_controller->set_state(PlantStates::INIT);
        Serial.println(F("Init"));
    }
    else if (strcmp(args[1], "notify") == 0)
    {   
        p_hydration_controller->set_state(PlantStates::NOTIFING);
        Serial.println(F("Notify"));
    }
    else if (strcmp(args[1], "monitor") == 0)
    {   
        p_hydration_controller->set_state(PlantStates::MONITORING);
        Serial.println(F("Monitor"));
    }
    else if (strcmp(args[1], "sleep") == 0)
    {   
        p_hydration_controller->set_state(PlantStates::SLEEP);
        Serial.println(F("Sleep"));
    }
  }
}

/**
 * @brief Initialize the controller. Which controls the states of the monitoring system.
 * 
 * @param p_hydration_player Pointer to dfplayer
 * @param p_hydration_monitor Pointer to the moiture sensor
 * @param p_hydration_presence Pointer to the movement sensor
*/
void HydrateController::begin(HydratePlayer *p_hydration_player, HydrateMonitor *p_hydration_monitor, HydratePresence *p_hydration_presence){
    this->p_hydration_player = p_hydration_player;
    this->p_hydration_monitor = p_hydration_monitor;
    this->p_hydration_presence = p_hydration_presence;

    p_hydration_controller = this;

    debug_timer.begin(PlantStates::DEBUG_TIME);
}

void HydrateController::set_state(PlantStates::MonitoringStates state){
    this->state = state;
}

/**
 * @brief Is part of the state machine and is triggered by the main loop when the system is in monitoring state
 * and the hydrate timer is finished. It checks the value of the moiture sensor and decides if the user needs to be notified.
 * When everything is fine it restarts the hydrate timer.
*/
void HydrateController::on_hydrate_timer_finished(){
    SerialLogger::p_serial_logger->log(F("Hydrate timer finished"), Debugger::DEBUG);
    if(p_hydration_monitor->readMoistureLevel() == hydrate_monitor::NULL_VALUE  ||
    p_hydration_monitor->readMoistureLevel() == hydrate_monitor::DRY ||
    p_hydration_monitor->readMoistureLevel()== hydrate_monitor::SOME_WATER){
        state = PlantStates::PRESENCE;
        presence_timer.start();
    }else{
      hydrate_timer.start();
    }
}

/**
 * @brief Is part of the state machine and is triggered by the main loop when the system is in presence state
 * it will check every two seconds if the user is still present. If the user is still present it will trigger the on_presence_detected.
 * If the user is not present it will restart the presence timer.
*/
void HydrateController::on_precence_timer_finished(){
    SerialLogger::p_serial_logger->log(F("Presence timer finished"), Debugger::DEBUG);
    if(p_hydration_presence->is_person_moving()){
        this->on_presence_detected();
    }else{
        presence_timer.start();
    }
}

/**
 * @brief Is part of the state machine and is triggered by the main loop when the system is in presence state
 * and the movement sensor detects movement. It will play the notification sound and set the state to NOTIFING.
*/
void HydrateController::on_presence_detected(){
    SerialLogger::p_serial_logger->log(F("Presence detected"), Debugger::WARNING);
    p_hydration_player->playTrack(1);
    state = PlantStates::NOTIFING;
}

/**
 * @brief Is part of the state machine and is triggered by the main loop when the system is in notifing state.
 * It will check every 500ms if the player is still playing. If the player is still playing it will restart the player polling timer.
*/
void HydrateController::on_notify(){
    SerialLogger::p_serial_logger->log(F("Notify finished"), Debugger::DEBUG);
    if(p_hydration_player->isPlaying()){
        player_polling_timer.start();
    }else{
        on_player_finished();
    }
}

/**
 * @brief Is part of the state machine and is triggered by the main loop when the system is in notifing state.
 * When the player is finished it will set the state to MONITORING and restart the hydrate timer.
*/
void HydrateController::on_player_finished(){
    SerialLogger::p_serial_logger->log(F("Player finished"), Debugger::DEBUG);
    state = PlantStates::MONITORING;
    hydrate_timer.start();
}

/**
 * @brief This function is used to print the internal values of the controller to the serial monitor.
*/
void HydrateController::print_state(){
    Serial.print(F("Controller: \nState:"));
    Serial.println(PlantStates::state_to_string(state));
    if(state == PlantStates::MONITORING){
        Serial.print(F("Moisture: "));
        Serial.print(F("MoistureLevel: "));
        Serial.println(hydrate_monitor::moistureLevelToString(p_hydration_monitor->plant_value.moisture_level));
        Serial.print(F("MoistureValue: "));
        Serial.println(p_hydration_monitor->plant_value.moisture_value);
    }else if(state == PlantStates::PRESENCE){
        Serial.print(F("Presence: "));
        Serial.print(F("Last measurement: "));
        Serial.println(String(p_hydration_presence->measurement.measured));
        Serial.print(F("Value: ") );
        Serial.println(String(p_hydration_presence->measurement.movement_sensor));
    }else if(state == PlantStates::NOTIFING){
        Serial.print(F("Player: "));
        Serial.println(p_hydration_player->isPlaying());
    }
}

/**
 * @brief This function decides what to do based on the current state of the controller.
 * And is called by the main loop of the program. It will also print the state of the controller to the serial monitor.
*/
void HydrateController::loop(){
    switch (state) {
        case PlantStates::INIT:
            //This is the first state of the where the timer is set to start the monitoring
            presence_timer.begin(PlantStates::PRESENCE_TIME);
            hydrate_timer.begin(PlantStates::MONITORING_TIME);
            player_polling_timer.begin(PlantStates::PLAYER_POLLING_TIME);
            hydrate_timer.start();
            presence_timer.start();

            state = PlantStates::MONITORING;
            break;
        case PlantStates::SLEEP:
            break;
        case PlantStates::MONITORING:
            // This is the state where the plant is being monitored
            // When the timer is finished, the plant will be monitored
            // If the plant is dry, the presence timer will be started
            // and the state will be changed to presence

            if(hydrate_timer.fire()){
              this->on_hydrate_timer_finished();
              this->print_state();
            }
            break;
        case PlantStates::PRESENCE:
          // This is the state where the presence is being detected
          // When a presence is detected, the player will be started
            if(presence_timer.fire()){
                on_precence_timer_finished();
            }
            break;
        case PlantStates::NOTIFING:
            // Monitor the player to know when it is finished
            if(player_polling_timer.fire()){
                on_notify();
            }
            break;
    }

    // if(debug_timer.fire()){
    //   print_state();
    //   debug_timer.start();
    // }
}