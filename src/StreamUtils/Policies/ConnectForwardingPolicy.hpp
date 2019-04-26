// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include <Client.h>

#include "../Configuration.hpp"

namespace StreamUtils {

struct ConnectForwardingPolicy {
  int connect(Client& target, const IPAddress& ip, uint16_t port) {
    return target.connect(ip, port);
  }

  int connect(Client& target, const char* ip, uint16_t port) {
    return target.connect(ip, port);
  }

  uint8_t connected(Client& target) {
    return target.connected();
  }

#if STREAMUTILS_CLIENT_STOP_TAKES_TIMEOUT
  bool stop(Client& target, unsigned timeout) {
    return target.stop(timeout);
  }
#else
  void stop(Client& target) {
    target.stop();
  }
#endif

  bool operator_bool(Client& target) {
    return target.operator bool();
  }
};

}  // namespace StreamUtils