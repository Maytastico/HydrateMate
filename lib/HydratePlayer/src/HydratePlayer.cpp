#include "HydratePlayer.hpp"

HydratePlayer::HydratePlayer(uint8_t rxPin, uint8_t txPin)
    : dfPlayerSerial(rxPin, txPin) {
}


void HydratePlayer::begin() {
  dfPlayerSerial.begin(9600); // Baudrate für SoftwareSerial setzen
  Serial.println();
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!dfplayer.begin(dfPlayerSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  dfplayer.play(1);
}



void HydratePlayer::loop(){
  
}