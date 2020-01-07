// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2020
// MIT License

#pragma once

#include <Client.h>

namespace StreamUtils {

class ReadSpyingPolicy {
 public:
  ReadSpyingPolicy(Print &log) : _log(log) {}

  int available(Stream &target) {
    int result = target.available();
    _log.print("available() -> ");
    _log.println(result);
    return result;
  }

  int read(Stream &target) {
    int result = target.read();
    _log.print("read() -> ");
    _log.println(result);
    return result;
  }

  int peek(Stream &target) {
    int result = target.peek();
    _log.print("peek() -> ");
    _log.println(result);
    return result;
  }

  size_t readBytes(Stream &target, char *buffer, size_t size) {
    size_t result = target.readBytes(buffer, size);
    _log.print("readBytes(");
    _log.print(size);
    _log.print(") -> ");
    _log.println(result);
    return result;
  }

  size_t read(Client &target, uint8_t *buffer, size_t size) {
    size_t result = target.read(buffer, size);
    _log.print("read(");
    _log.print(size);
    _log.print(") -> ");
    _log.println(result);
    return result;
  }

 private:
  Print &_log;
};

}  // namespace StreamUtils
