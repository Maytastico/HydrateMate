#include "HydratePlayer.hpp"

HydratePlayer::HydratePlayer(uint8_t rxPin, uint8_t txPin)
    : dfPlayerSerial(rxPin, txPin) {
}

void HydratePlayer::playTrack(uint16_t track_number){
  dfplayer.normalMode();
  dfplayer.play(track_number);

}


void HydratePlayer::begin() {
  Serial.println(F("Initializing DFPlayer"));
  dfPlayerSerial.begin(9600);
  dfplayer.begin(dfPlayerSerial);
  dfplayer.volume(30);
  dfplayer.play(1);
}

bool HydratePlayer::isPlaying(){
  return dfplayer.isPlaying();
}


void HydratePlayer::loop(){
    if(!dfplayer.isPlaying()){
      dfplayer.sleep();
    }
}