// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include <Stream.h>

namespace StreamUtils {

struct ReadForwardingPolicy {
  int available(Stream &stream) {
    return stream.available();
  }

  int read(Stream &stream) {
    return stream.read();
  }

  int peek(Stream &stream) {
    return stream.peek();
  }

  size_t readBytes(Stream &stream, char *buffer, size_t size) {
    return stream.readBytes(buffer, size);
  }
};

}  // namespace StreamUtils