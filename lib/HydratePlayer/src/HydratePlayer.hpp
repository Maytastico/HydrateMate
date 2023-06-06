#ifndef HYDRATEPLAYER_HPP
#define HYDRATEPLAYER_HPP

#include <Arduino.h>
#include <SoftwareSerial.h> // SoftwareSerial-Bibliothek hinzufügen
#include <DFPlayerMini_Fast.h>

class HydratePlayer {
public:
  HydratePlayer(uint8_t rxPin, uint8_t txPin); // Konstruktor hinzugefügt
  void begin();
  void playTrack(uint8_t trackNumber);
  void stop();
  void loop();

private:
  SoftwareSerial dfPlayerSerial; // SoftwareSerial-Instanz hinzugefügt
  DFPlayerMini_Fast dfplayer;
};

#endif  // HYDRATEPLAYER_HPP

