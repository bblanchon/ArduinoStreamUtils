// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2024
// MIT License

#pragma once

#include "StreamUtils/Policies/ReadForwardingPolicy.hpp"
#include "StreamUtils/Policies/WriteForwardingPolicy.hpp"

#include "StreamProxy.hpp"

namespace StreamUtils {

struct ChunkDecodingStream
    : StreamProxy<ReadForwardingPolicy, WriteForwardingPolicy> {
  ChunkDecodingStream(Stream &target)
      : StreamProxy<ReadForwardingPolicy, WriteForwardingPolicy>(target) {}
};

}  // namespace StreamUtils
