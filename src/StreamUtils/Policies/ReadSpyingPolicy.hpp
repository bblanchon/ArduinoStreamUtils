// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include <Stream.h>

namespace StreamUtils {

class ReadSpyingPolicy {
 public:
  ReadSpyingPolicy(Stream &log) : _log(log) {}

  int available(Stream &stream) {
    int result = stream.available();
    _log.print("available() -> ");
    _log.println(result);
    return result;
  }

  int read(Stream &stream) {
    int result = stream.read();
    _log.print("read() -> ");
    _log.println(result);
    return result;
  }

  int peek(Stream &stream) {
    int result = stream.peek();
    _log.print("peek() -> ");
    _log.println(result);
    return result;
  }

  size_t readBytes(Stream &stream, char *buffer, size_t size) {
    size_t result = stream.readBytes(buffer, size);
    _log.print("readBytes(");
    _log.print(size);
    _log.print(") -> ");
    _log.println(result);
    return result;
  }

 private:
  Stream &_log;
};

}  // namespace StreamUtils
