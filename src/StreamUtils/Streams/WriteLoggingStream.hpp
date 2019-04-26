// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "../Policies/ReadForwardingPolicy.hpp"
#include "../Policies/WriteLoggingPolicy.hpp"
#include "StreamProxy.hpp"

namespace StreamUtils {

struct WriteLoggingStream
    : StreamProxy<ReadForwardingPolicy, WriteLoggingPolicy> {
  WriteLoggingStream(Stream &upstream, Stream &log)
      : StreamProxy<ReadForwardingPolicy, WriteLoggingPolicy>(
            upstream, ReadForwardingPolicy{}, WriteLoggingPolicy{log}) {}
};

}  // namespace StreamUtils