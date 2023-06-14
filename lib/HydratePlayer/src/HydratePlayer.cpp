#include "HydratePlayer.hpp"

/**
 * @brief Static instance of the DFPlayerMini_Fast library.
*/
DFPlayerMini_Fast HydratePlayer::dfplayer;

/**
 * @brief Constructor for HydratePlayer. In addition it holds data about the sounds and the volume.
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
  dfplayer.begin(dfPlayerSerial, true);
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
 * @brief Handles serial commands reagarding the DFPlayer Mini.
*/
void HydratePlayer::serialHandler(int argCnt, char **args){
  if(strcmp(args[0], "t") == 0){
    int track = cmd.conv(args[1]);
    dfplayer.play(track);
    Serial.print(F("Playing Track: "));
    Serial.println(track);
  }
  else if (strcmp(args[0], "v") == 0)
  {
    int volume = cmd.conv(args[1]);
    dfplayer.volume(volume);
    Serial.print(F("Volume: "));
    Serial.println(volume);
  }

  if (strcmp(args[0], "p") == 0)
  {
    if(strcmp(args[1], "s") == 0){
      dfplayer.stop();
      Serial.print(F("Stopping Track"));
    }else if(strcmp(args[1], "st") == 0){
      Serial.print(F("Is Playing: "));
      Serial.println(dfplayer.isPlaying());
    }
  }
}

/**
 * @brief Sets the DFPlayer Mini to sleep mode, when playing is finished.
*/
void HydratePlayer::loop(){
    if(!dfplayer.isPlaying()){
      dfplayer.parseFeedback();
    }
}