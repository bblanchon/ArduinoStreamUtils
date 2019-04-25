// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "ReadForwardingPolicy.hpp"
#include "StreamProxy.hpp"
#include "WriteLoggingPolicy.hpp"

namespace StreamUtils {

struct WriteLoggingStream
    : public StreamProxy<ReadForwardingPolicy, WriteLoggingPolicy> {
  WriteLoggingStream(Stream &upstream, Stream &log)
      : StreamProxy<ReadForwardingPolicy, WriteLoggingPolicy>(
            upstream, ReadForwardingPolicy{}, WriteLoggingPolicy{log}) {}
};

}  // namespace StreamUtils