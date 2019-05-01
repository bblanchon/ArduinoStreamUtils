// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "Print.h"

struct Stream : Print {
  virtual int available() = 0;
  virtual int read() = 0;
  virtual int peek() = 0;
  virtual void flush() = 0;

  size_t readBytes(char *buffer, size_t length) {
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
