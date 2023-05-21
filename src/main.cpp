#include <Arduino.h>
#include <HydratePlayer.hpp>
#include <HydrateMonitor.hpp>
#include <HydratePresence.hpp>


HydratePlayer player(10,11);
HydrateMonitor plant_monitor(A1, 9);
HydratePresence presence_detection(8);

void setup() {
  Serial.begin(115200);

  player.begin();

  plant_monitor.begin();

  plant_monitor.readMoistureLevel();

  presence_detection.begin();
  presence_detection.isPersonMoving();
}

void loop() {
  if(millis() > presence_detection.getTimeOfLastMeasurement()+2000){
    presence_detection.isPersonMoving();
  }
}