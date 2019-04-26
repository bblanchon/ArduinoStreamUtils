// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

namespace StreamUtils {

class WriteSpyingPolicy {
 public:
  WriteSpyingPolicy(Stream &log) : _log(log) {}

  size_t write(Stream &stream, const uint8_t *buffer, size_t size) {
    size_t result = stream.write(buffer, size);
    _log.print("write('");
    for (size_t i = 0; i < size; i++) {
      _log.write(buffer[i]);
    }
    _log.print("', ");
    _log.print(size);
    _log.print(") -> ");
    _log.println(result);
    return result;
  }

  size_t write(Stream &stream, uint8_t data) {
    size_t result = stream.write(data);
    _log.print("write('");
    _log.write(data);
    _log.print("') -> ");
    _log.println(result);
    return result;
  }

  void flush(Stream &stream) {
    _log.println("flush()");
    stream.flush();
  }

  void detach(Print &) {}

 private:
  Stream &_log;
};

}  // namespace StreamUtils
