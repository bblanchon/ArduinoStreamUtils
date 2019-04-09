// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include <stdint.h>

struct Print {
  virtual size_t write(const uint8_t *buffer, size_t size) = 0;
  virtual size_t write(uint8_t data) = 0;

  size_t write(const char *buffer, size_t size) {
    return write((const uint8_t *)buffer, size);
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
      if (c < 0) break;
      *buffer++ = (char)c;
      count++;
    }
    return count;
  }
};
