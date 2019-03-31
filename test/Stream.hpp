// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include <stdint.h>

struct Print {
  virtual size_t write(const uint8_t *buffer, size_t size) = 0;
  virtual size_t write(uint8_t data) = 0;
};

struct Stream : Print {
  virtual int available() = 0;
  virtual int read() = 0;
  virtual int peek() = 0;
  virtual void flush() = 0;

  // WARNING: most cores don't define this function as virtual
  size_t readBytes(char *buffer, size_t length);
};