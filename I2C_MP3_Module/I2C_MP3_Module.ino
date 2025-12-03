/**
 * @copyright GPLv3
 * I2C address: 0x35
 */

#if defined(ARDUINO_ADAFRUIT_QTPY_RP2040)
  #define PIN_DATA  28
  #define PIN_WS    27
  #define PIN_BCK   26
  #define PIN_SD_CS 29
#elif defined(ARDUINO_SEEED_XIAO_RP2040)
  #define PIN_DATA  27
  #define PIN_WS    28
  #define PIN_BCK   29
  #define PIN_SD_CS 26
#endif

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include <EEPROM.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>


// setup
String empty = "";
AudioInfo info(44100, 2, 16);
I2SStream i2s;
VolumeStream volume(i2s);
MP3DecoderHelix mp3;
EncodedAudioStream decoder(&volume, &mp3);

// play
File audioFile;
StreamCopy copier;
String playNext = empty;

// messaging
String ready = "{ \"ready\": 1 }";
String not_ready = "{ \"ready\": 0 }";
String state = ready;
String i2cResponse = empty;

void setup() {  
  //Serial.begin(115200);
  prepareOutput();
  prepareFilesystem();
  prepareI2C(); // Processes I2C command separately.
  disableLEDs();
}

void loop() {
  if (copier.copy()) {
    return;
  }
  audioFile.close();
  startPlaying();
}
