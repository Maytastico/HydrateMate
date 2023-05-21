#ifndef HYDRATEPLAYER_HPP
#define HYDRATEPLAYER_HPP

#include <Arduino.h>
#include <SoftwareSerial.h> // SoftwareSerial-Bibliothek hinzufügen
#include <DFRobotDFPlayerMini.h>

class HydratePlayer {
public:
  HydratePlayer(uint8_t rxPin, uint8_t txPin); // Konstruktor hinzugefügt
  void begin();
  void playTrack(uint8_t trackNumber);
  void stop();
  void loop();
  void printStatus(uint8_t type, int value);

private:
  SoftwareSerial dfPlayerSerial; // SoftwareSerial-Instanz hinzugefügt
  DFRobotDFPlayerMini dfplayer;
};

#endif  // HYDRATEPLAYER_HPP

