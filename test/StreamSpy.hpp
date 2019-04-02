// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "Stream.hpp"

#include <sstream>
#include <string>

class StreamSpy : public Stream {
 public:
  StreamSpy(Stream &upstream) : _upstream(upstream) {}

  virtual size_t write(const uint8_t *buffer, size_t size) {
    size_t result = _upstream.write(buffer, size);
    _log << "write(" << size << ") -> " << result;
    return result;
  }

  virtual size_t write(uint8_t data) {
    size_t result = _upstream.write(data);
    _log << "write(1) -> " << result;
    return result;
  }

  virtual int available() {
    int result = _upstream.available();
    _log << "available() -> " << result;
    return result;
  }

  virtual int read() {
    int result = _upstream.read();
    _log << "read() -> " << result;
    return result;
  }

  virtual int peek() {
    int result = _upstream.peek();
    _log << "peek() -> " << result;
    return result;
  }

  virtual void flush() {
    _log << "flush()";
    _upstream.flush();
  }

  size_t readBytes(char *buffer, size_t size) {
    size_t result = _upstream.readBytes(buffer, size);
    _log << "readBytes(" << size << ") -> " << result;
    return result;
  }

  std::string log() const {
    return _log.str();
  }

 private:
  std::stringstream _log;
  Stream &_upstream;
};
