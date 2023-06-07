#include <Arduino.h>
#include <HydratePlayer.hpp>
#include <HydrateMonitor.hpp>
#include <HydratePresence.hpp>
#include <HydrateController.hpp>


HydratePlayer player(10,11);
HydrateMonitor plant_monitor(A1, 9);
HydratePresence presence_detection(8);

HydrateController hydrate_controller;

void setup() {
  Serial.begin(115200);


  player.begin();

  plant_monitor.begin();

  plant_monitor.readMoistureLevel();

  presence_detection.begin();

  hydrate_controller.begin(&player, &plant_monitor, &presence_detection);
}

void loop() {
  hydrate_controller.loop();
  player.loop();
}