#ifndef HYDRATEPLAYER_HPP
#define HYDRATEPLAYER_HPP

#include <Arduino.h>
#include <SoftwareSerial.h> // SoftwareSerial-Bibliothek hinzufügen
#include <DFPlayerMini_Fast.h>
#include <cmdArduino.h>

#define DFPLAYER_VOLUME 100

class HydratePlayer {
public:
  HydratePlayer(uint8_t rxPin, uint8_t txPin); // Konstruktor hinzugefügt
  void begin();
  void playTrack(uint16_t trackNumber);
  void stop();
  void loop();
  bool isPlaying();
  static void serialHandler(int argCnt, char **args);

private:

  SoftwareSerial dfPlayerSerial; // SoftwareSerial-Instanz hinzugefügt
  static DFPlayerMini_Fast dfplayer;
};

#endif  // HYDRATEPLAYER_HPP

