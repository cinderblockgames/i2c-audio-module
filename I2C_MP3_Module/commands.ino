void processI2C(int numBytes) {
  // { "command": "get_state/get_files_length/list_files/stop_playing" }
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
  if      (command == "get_state")        processState();
  else if (command == "get_files_length") processFilesLength();
  else if (command == "list_files")       processListFiles();
  else if (command == "stop_playing")     processStop();
  else if (command == "play_next")        processPlayNext(message["file"]);
  else if (command == "set_volume")       processVolume(atoi(message["volume"]));;
}

void processState() {
  #if defined(ARDUINO_ADAFRUIT_QTPY_RP2040)
    Wire1.write(state.c_str());
  #elif defined(ARDUINO_SEEED_XIAO_RP2040)
    Wire.write(state.c_str());
  #endif
  print(state);
}

void processFilesLength() {
  #if defined(ARDUINO_ADAFRUIT_QTPY_RP2040)
    Wire1.write(filesLength.c_str());
  #elif defined(ARDUINO_SEEED_XIAO_RP2040)
    Wire.write(filesLength.c_str());
  #endif
  print(filesLength);
}

void processListFiles() {
  #if defined(ARDUINO_ADAFRUIT_QTPY_RP2040)
    Wire1.write(files.c_str());
  #elif defined(ARDUINO_SEEED_XIAO_RP2040)
    Wire.write(files.c_str());
  #endif
  print(files);
}

void processStop() {
  stopFlag = true;
}

void processPlayNext(String filepath) {
  setNext(filepath);
}

void processVolume(int newVolume) {
  if (newVolume > 0 && newVolume <= 100) {
    volume.setVolume((float)newVolume / 100.00f);
  }
}
