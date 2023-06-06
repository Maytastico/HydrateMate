#include <HydrateController.hpp>

namespace PlantStates
{
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
} // namespace Pla


void HydrateController::begin(HydratePlayer *p_hydration_player, HydrateMonitor *p_hydration_monitor, HydratePresence *p_hydration_presence){
    this->p_hydration_player = p_hydration_player;
    this->p_hydration_monitor = p_hydration_monitor;
    this->p_hydration_presence = p_hydration_presence;

    debug_timer.begin(PlantStates::DEBUG_TIME);
}

void HydrateController::on_hydrate_timer_finished(){
    Serial.println(F("Hydrate timer finished"));
    if(p_hydration_monitor->readMoistureLevel() == NULL_VALUE  ||
    p_hydration_monitor->readMoistureLevel() == DRY ||
    p_hydration_monitor->readMoistureLevel()== SOME_WATER){
        state = PlantStates::PRESENCE;
        presence_timer.start();
    }else{
      hydrate_timer.start();
    }
}

void HydrateController::on_precence_timer_finished(){
    Serial.println(F("Presence timer finished"));
    if(p_hydration_presence->isPersonMoving()){
        this->on_presence_detected();
    }else{
        presence_timer.start();
    }
}

void HydrateController::on_presence_detected(){
    Serial.println(F("Presence detected"));
    p_hydration_player->playTrack(1);
    state = PlantStates::NOTIFING;
}

void HydrateController::on_notify(){
    Serial.println(F("Notify finished"));
    if(p_hydration_player->isPlaying()){
        player_polling_timer.start();
    }else{
        on_player_finished();
    }
}

void HydrateController::on_player_finished(){
    Serial.println(F("Player finished"));
    state = PlantStates::MONITORING;
    hydrate_timer.start();
}

void HydrateController::print_state(){
    Serial.print(F("Controller: \nState:"));
    Serial.println(PlantStates::state_to_string(state));
    if(state == PlantStates::MONITORING){
        Serial.print(F("Moisture: "));
        Serial.print(F("MoistureLevel: "));
        Serial.println(p_hydration_monitor->plant_value.moisture_level);
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

void HydrateController::loop(){
    switch (state) {
        case PlantStates::INIT:
            //This is the first state of the where the timer is set to start the monitoring
            presence_timer.begin(PlantStates::PRESENCE_TIME);
            hydrate_timer.begin(PlantStates::MONITORING_TIME);
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

    if(debug_timer.fire()){
      print_state();
      debug_timer.start();
    }
}