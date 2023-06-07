#ifndef HydrateController_HPP
#define HydrateController_HPP

#include <Arduino.h>
#include <FireTimer.h>

#include "HydrateMonitor.hpp"
#include "HydratePlayer.hpp"
#include "HydratePresence.hpp"



namespace PlantStates{

  const int32_t MONITORING_TIME = 120000;
  const int32_t PRESENCE_TIME = 2000;
  const int32_t PLAYER_POLLING_TIME = 500;

  const int32_t DEBUG_TIME = 1000;

  enum MonitoringStates{
    INIT,
    SLEEP,
    MONITORING,
    PRESENCE,
    NOTIFING,
  };

  String state_to_string(MonitoringStates state);

}


class HydrateController {
public:
  HydrateController(){};
  void begin(HydratePlayer *p_hydration_player, HydrateMonitor *p_hydration_monitor, HydratePresence *p_hydration_presence);
  void loop();

  void print_state();
  

private:
    FireTimer hydrate_timer;
    FireTimer presence_timer;
    FireTimer player_polling_timer;
    FireTimer debug_timer;

    HydrateMonitor *p_hydration_monitor = nullptr;
    HydratePresence *p_hydration_presence = nullptr;
    HydratePlayer *p_hydration_player = nullptr;

    PlantStates::MonitoringStates state = PlantStates::INIT;

    void on_presence_detected();
    void on_player_finished();
    void on_notify();
    void on_hydrate_timer_finished();
    void on_precence_timer_finished();


};

#endif  // HydrateController_HPP

