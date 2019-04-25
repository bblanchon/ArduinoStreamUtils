// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

namespace StreamUtils {

struct WriteForwardingPolicy {
  size_t write(Stream &stream, const uint8_t *buffer, size_t size) {
    return stream.write(buffer, size);
  }

  size_t write(Stream &stream, uint8_t data) {
    return stream.write(data);
  }

  void flush(Stream &stream) {
    stream.flush();
  }

  void detach(Stream &) {}
};

}  // namespace StreamUtils