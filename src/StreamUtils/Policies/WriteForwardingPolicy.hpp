// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include <Stream.h>

namespace StreamUtils {

struct WriteForwardingPolicy {
  template <typename... Args>
  size_t write(Stream &stream, Args... args) {
    return stream.write(args...);
  }

  void flush(Stream &stream) {
    stream.flush();
  }

  void detach(Stream &) {}
};

}  // namespace StreamUtils