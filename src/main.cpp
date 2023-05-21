#include <Arduino.h>
#include <HydratePlayer.hpp>
#include <HydrateMonitor.hpp>


HydratePlayer player(10,11);
HydrateMonitor plant_monitor(A1, 9);

void setup() {
  Serial.begin(115200);

  player.begin();

  plant_monitor.begin();

  plant_monitor.readMoistureLevel();
}

void loop() {
  
}