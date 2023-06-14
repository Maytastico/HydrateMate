#include <Arduino.h>
#include <HydratePlayer.hpp>
#include <HydrateMonitor.hpp>
#include <HydratePresence.hpp>
#include <HydrateController.hpp>
#include <SerialLogger.hpp>
#include <cmdArduino.h>


HydratePlayer player(10,11);
HydrateMonitor plant_monitor(A1, 9);
HydratePresence presence_detection(8);

HydrateController hydrate_controller;

SerialLogger logger;

void arg_display(int argCnt, char **args);

void setup() {
  Serial.begin(115200);
  cmd.begin(115200);

  player.begin();

  plant_monitor.begin();

  plant_monitor.readMoistureLevel();

  presence_detection.begin();

  hydrate_controller.begin(&player, &plant_monitor, &presence_detection);

  cmd.add("m", hydrate_controller.serialHandler);
  cmd.add("d", hello);
  cmd.add("p", player.serialHandler);
  cmd.add("v", player.serialHandler);
  cmd.add("t", player.serialHandler);
  cmd.add("args", arg_display);

  player.playTrack(1);
}

void loop() {
  hydrate_controller.loop();

  cmd.poll();
  //player.loop();
}

void arg_display(int argCnt, char **args)
{
  for (int i=0; i<argCnt; i++)
  {
    Serial.print("Arg ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(args[i]);
  }
}