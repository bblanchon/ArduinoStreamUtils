// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include <stdint.h>
#include <cstring>
#include <string>

using String = std::string;

struct Print {
  virtual size_t write(const uint8_t *buffer, size_t size) = 0;
  virtual size_t write(uint8_t data) = 0;

  size_t write(const char *buffer, size_t size) {
    return write((const uint8_t *)buffer, size);
  }

  size_t print(const String &s) {
    return write(s.c_str(), s.size());
  }

  size_t print(const char *s) {
    return write(s, std::strlen(s));
  }

  template <typename T>
  size_t print(const T &value) {
    return print(std::to_string(value));
  }
};

struct Stream : Print {
  virtual int available() = 0;
  virtual int read() = 0;
  virtual int peek() = 0;
  virtual void flush() = 0;

  // NOTE: most cores don't define this function as virtual
  //
  // | Core    | Virtual? |
  // |---------|----------|
  // | AVR     | No       |
  // | SAMD    | No       |
  // | ESP8266 | Yes      |
  //
  virtual size_t readBytes(char *buffer, size_t length) {
    size_t count = 0;
    while (count < length) {
      int c = read();
      if (c < 0)
        break;
      *buffer++ = (char)c;
      count++;
    }
    return count;
  }

  String readString() {
    size_t n = available();
    std::string result;
    result.resize(n);
    n = readBytes(&result[0], n);
    result.resize(n);
    return result;
  }
};
