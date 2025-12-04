void processI2C(int numBytes) {
  // { "command": "get_state/list_files/stop_playing/get_response" }
  // { "command": "play_next", "file": "/file.mp3" }
  // { "command": "set_volume", "volume": "int" } 1-100

  char str[numBytes + 1];
  for (int i = 0; i < numBytes; i++) {
    #if defined(ARDUINO_ADAFRUIT_QTPY_RP2040)
      str[i] = char(Wire1.read());
    #elif defined(ARDUINO_SEEED_XIAO_RP2040)
      str[i] = char(Wire.read());
    #endif
  }
  str[numBytes] = '\0';

  JsonDocument message;
  deserializeJson(message, str);

  auto command = message["command"];
  print(str);
  if      (command == "get_state")       processState();
  else if (command == "list_files")      processListFiles();
  else if (command == "play_next")       processPlayNext(message["file"]);
  else if (command == "stop_playing")    processStop();
  else if (command == "set_volume")      processVolume(atoi(message["volume"]));
  else if (command == "get_response")    processResponse();
}

void processState() {
  #if defined(ARDUINO_ADAFRUIT_QTPY_RP2040)
    Wire1.write(state.c_str());
  #elif defined(ARDUINO_SEEED_XIAO_RP2040)
    Wire.write(state.c_str());
  #endif
  print(state);
}

void processListFiles() {
  write(listFilesAsJson());
}

void processPlayNext(String filepath) {
  setNext(filepath);
}

void processStop() {
  stopFlag = true;
}

void processVolume(int newVolume) {
  if (newVolume > 0 && newVolume <= 100) {
    volume.setVolume((float)newVolume / 100.00f);
  }
}

void processResponse() {
  #if defined(ARDUINO_ADAFRUIT_QTPY_RP2040)
    Wire1.write(i2cResponse.c_str());
  #elif defined(ARDUINO_SEEED_XIAO_RP2040)
    Wire.write(i2cResponse.c_str());
  #endif
  print(i2cResponse);
  i2cResponse = empty;
}

void write(String message) {
  String length = "0000000000" + String(message.length());
  length = length.substring(length.length()-10);

  print(length);
  #if defined(ARDUINO_ADAFRUIT_QTPY_RP2040)
    Wire1.write(length.c_str());
  #elif defined(ARDUINO_SEEED_XIAO_RP2040)
    Wire.write(length.c_str());
  #endif

  i2cResponse = message;
}
