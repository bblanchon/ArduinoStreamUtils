// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2024
// MIT License

#pragma once

#include "StreamUtils/Policies/ChunkDecodingPolicy.hpp"
#include "StreamUtils/Policies/WriteForwardingPolicy.hpp"

#include "StreamProxy.hpp"

namespace StreamUtils {

struct ChunkDecodingStream
    : StreamProxy<ChunkDecodingPolicy, WriteForwardingPolicy> {
  ChunkDecodingStream(Stream &target)
      : StreamProxy<ChunkDecodingPolicy, WriteForwardingPolicy>(target) {}
};

}  // namespace StreamUtils
