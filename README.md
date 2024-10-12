# Implement a functional ADC for ESP32-C6 in Esphome

This is just a work-around to get ADC working on the ESP32-C6 in esphome

Valid ADC pins are GPIO0-GPIO6

A typical configuration would look like:
```
esp32:
  board: esp32-c6-devkitc-1
  flash_size: 4MB
  variant: esp32c6
  framework:
    type: esp-idf
    version: "5.3.0"
    platform_version: 6.8.1
    sdkconfig_options:
      CONFIG_ESPTOOLPY_FLASHSIZE_8MB: y

external_components:
  - source: github://PhracturedBlue/c6_adc

sensor:
  - platform: c6_adc
    name: adc5
    pin: GPIO5
    attenuation: 0dB  # optional.  defaults to 12dB
```
