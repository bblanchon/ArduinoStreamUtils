// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "../Policies/ReadSpyingPolicy.hpp"
#include "../Policies/WriteSpyingPolicy.hpp"
#include "StreamProxy.hpp"

namespace StreamUtils {

struct SpyingStream : StreamProxy<ReadSpyingPolicy, WriteSpyingPolicy> {
  SpyingStream(Stream &upstream, Stream &log)
      : StreamProxy<ReadSpyingPolicy, WriteSpyingPolicy>(upstream, {log},
                                                         {log}) {}
};

}  // namespace StreamUtils
