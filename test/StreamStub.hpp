// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "Stream.hpp"

class StreamStub : public Stream {
 public:
  void setup(const std::string &s) {
    _data.str(s);
    _available = s.length();
  }

  virtual size_t write(const uint8_t *, size_t size) {
    return size;
  }
  virtual size_t write(uint8_t) {
    return 1;
  }
  virtual int available() {
    return _available;
  }
  virtual int read() {
    _available--;
    return _data.get();
  }
  virtual int peek() {
    return _data.peek();
  }
  virtual void flush() {}

  size_t readBytes(char *buffer, size_t size) {
    _data.read(buffer, size);
    _available -= _data.gcount();
    return _data.gcount();
  }

 private:
  std::stringstream _data;
  int _available;
};
