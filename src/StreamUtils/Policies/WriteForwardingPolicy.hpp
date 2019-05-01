// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include <Client.h>
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

#if STREAMUTILS_CLIENT_FLUSH_TAKES_TIMEOUT
  bool flush(Client &client, unsigned timeout) {
    return client.flush(timeout);
  }
#endif

  void implicitFlush(Stream &) {}
};

}  // namespace StreamUtils