// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

namespace StreamUtils {

class WriteLoggingPolicy {
 public:
  WriteLoggingPolicy(Stream &log) : _log(log) {}

  size_t write(Stream &stream, const uint8_t *buffer, size_t size) {
    size_t result = stream.write(buffer, size);
    _log.write(buffer, result);
    return result;
  }

  size_t write(Stream &stream, uint8_t c) {
    size_t result = stream.write(c);
    _log.write(c);
    return result;
  }

  void flush(Stream &stream) {
    stream.flush();
    _log.flush();
  }

  void detach(Stream &) {}

 private:
  Stream &_log;
};

}  // namespace StreamUtils