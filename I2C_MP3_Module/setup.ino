void prepareOutput() {
  auto config = i2s.defaultConfig(TX_MODE);
  config.pin_data = PIN_DATA;
  config.pin_ws   = PIN_WS;
  config.pin_bck  = PIN_BCK;
  config.copyFrom(info);
  i2s.begin(config);

  auto vcfg = volume.defaultConfig();
  vcfg.copyFrom(config);
  volume.begin(vcfg);

  decoder.begin();
}

void prepareFilesystem() {  
  SD.begin(PIN_SD_CS);

  files = listFilesAsJson();
  String length = "0000000000" + String(files.length());
  filesLength = length.substring(length.length()-10);
}

void prepareI2C() {
  #if defined(ARDUINO_ADAFRUIT_QTPY_RP2040)
    pinMode(22, INPUT_PULLUP);
    pinMode(23, INPUT_PULLUP);
    Wire1.begin(0x35);
    Wire1.onReceive(processI2C);
  #elif defined(ARDUINO_SEEED_XIAO_RP2040)
    pinMode(6, INPUT_PULLUP);
    pinMode(7, INPUT_PULLUP);
    Wire.begin(0x35);
    Wire.onReceive(processI2C);
  #endif
}

void disableLEDs() {
  #if defined(ARDUINO_SEEED_XIAO_RP2040)
    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    digitalWrite(PIN_LED_R, HIGH);
    digitalWrite(PIN_LED_G, HIGH);
    digitalWrite(PIN_LED_B, HIGH);
    // no way to turn off the power LED
  #endif
}
