// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2021
// MIT License

#pragma once

#include "../Configuration.hpp"

#if STREAMUTILS_ENABLE_EEPROM

#include <EEPROM.h>
#include <Stream.h>

namespace StreamUtils {

class EepromStream : public Stream {
 public:
  EepromStream(size_t address, size_t size)
      : _readAddress(address), _writeAddress(address), _end(address + size) {}

  int available() override {
    return _end - _readAddress;
  }

  int read() override {
    if (_readAddress >= _end)
      return -1;
    return EEPROM.read(_readAddress++);
  }

  int peek() override {
    if (_readAddress >= _end)
      return -1;
    return EEPROM.read(_readAddress);
  }

  void flush() override {
#if STREAMUTILS_USE_EEPROM_COMMIT
    EEPROM.commit();
#endif
  }

  using Print::write;

  size_t write(const uint8_t *buffer, size_t size) override {
    size_t remaining = _end - _writeAddress;
    if (size > remaining)
      size = remaining;
    for (size_t i = 0; i < size; i++) {
#if STREAMUTILS_USE_EEPROM_UPDATE
      EEPROM.update(_writeAddress++, buffer[i]);
#else
      EEPROM.write(_writeAddress++, buffer[i]);
#endif
    }
    return size;
  }

  size_t write(uint8_t data) override {
    if (_writeAddress >= _end)
      return 0;
#if STREAMUTILS_USE_EEPROM_UPDATE
    EEPROM.update(_writeAddress++, data);
#else
    EEPROM.write(_writeAddress++, data);
#endif
    return 1;
  }

 private:
  size_t _readAddress, _writeAddress, _end;
};

}  // namespace StreamUtils

#endif
