// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "ReadLoggingPolicy.hpp"
#include "StreamProxy.hpp"
#include "WriteForwardingPolicy.hpp"

namespace StreamUtils {

struct ReadLoggingStream
    : StreamProxy<ReadLoggingPolicy, WriteForwardingPolicy> {
  ReadLoggingStream(Stream &upstream, Stream &log)
      : StreamProxy<ReadLoggingPolicy, WriteForwardingPolicy>(
            upstream, ReadLoggingPolicy{log}, WriteForwardingPolicy{}) {}
};
}  // namespace StreamUtils