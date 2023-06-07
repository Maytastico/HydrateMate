#include "HydratePlayer.hpp"

/**
 * @brief Constructor for HydratePlayer. 
 * Which uses SoftwareSerial for communication to DFPlayer Mini and
 * the DFPlayerMini_Fast library for sending the commands.
 * @param rxPin Pin for RX
 * @param txPin Pin for TX
*/
HydratePlayer::HydratePlayer(uint8_t rxPin, uint8_t txPin)
    : dfPlayerSerial(rxPin, txPin) {
}

/**
 * @brief Initializes the DFPlayer Mini. Sets the volume to 30 and plays track 1 to say hello.
*/
void HydratePlayer::begin() {
  Serial.println(F("Initializing DFPlayer"));
  dfPlayerSerial.begin(9600);
  dfplayer.begin(dfPlayerSerial);
  dfplayer.reset();
  dfplayer.volume(29);
  dfplayer.play(1);
}

/**
 * @brief Plays a track on the DFPlayer Mini. It sets the DFPlayer Mini to normal mode.
 * @param trackNumber Number of the track to play
*/
void HydratePlayer::playTrack(uint16_t track_number){
  dfplayer.normalMode();
  dfplayer.play(track_number);

}

/**
 * @brief Returns true if the DFPlayer Mini is playing a track.
*/
bool HydratePlayer::isPlaying(){
  return dfplayer.isPlaying();
}

/**
 * @brief Sets the DFPlayer Mini to sleep mode, when playing is finished.
*/
void HydratePlayer::loop(){
    if(!dfplayer.isPlaying()){
      //dfplayer.sleep();
    }
}