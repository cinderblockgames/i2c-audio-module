# i2c-audio-module
Code and design for an Arduino audio peripheral that can be managed over I2C for use with any language controller.

This module uses [arduino-audio-tools](https://github.com/pschatzmann/arduino-audio-tools/), [arduino-libhelix](https://github.com/pschatzmann/arduino-libhelix/) and [ArduinoJson](https://github.com/bblanchon/ArduinoJson).

# Physical build
The physical components of this build are an [Adafruit Audio BFF](https://www.adafruit.com/product/5769) and either an [Adafruit QT Py RP2040](https://www.adafruit.com/product/4900) or a [Seeed Studio XIAO RP2040](https://www.seeedstudio.com/XIAO-RP2040-v1-0-p-5026.html), attached back to back.

Benefits of using the QT Py are 1) no-solder I2C connection over STEMMA QT, and 2) power can be supplied to the module over I2C.  
Benefits of using the XIAO are 1) cheaper.  However, power has to be provided directly to the XIAO.  If powering only the XIAO and allowing the controller to be powered by it, then connect all four I2C cables.  If powering both the XIAO and the controller separately, do not connect power between the two boards.
