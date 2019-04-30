// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include <Stream.h>

namespace StreamUtils {

class WriteSpyingPolicy {
 public:
  WriteSpyingPolicy(Print &log) : _log(log) {}

  size_t write(Stream &stream, const uint8_t *buffer, size_t size) {
    _log.print("write('");
    for (size_t i = 0; i < size; i++) {
      _log.write(buffer[i]);
    }
    _log.print("', ");
    _log.print(size);
    _log.print(") -> ");

    size_t result = stream.write(buffer, size);
    _log.println(result);

    return result;
  }

  size_t write(Stream &stream, uint8_t data) {
    _log.print("write('");
    _log.write(data);
    _log.print("') -> ");

    size_t result = stream.write(data);
    _log.println(result);

    return result;
  }

  void flush(Stream &stream) {
    _log.println("flush()");
    stream.flush();
  }

#if STREAMUTILS_CLIENT_FLUSH_TAKES_TIMEOUT
  bool flush(Client &client, unsigned timeout) {
    _log.print("flush(");
    _log.print(timeout);
    _log.print(") -> ");

    bool result = client.flush(timeout);
    _log.println(result);

    return result;
  }
#endif

  void detach(Print &) {}

 private:
  Print &_log;
};

}  // namespace StreamUtils
