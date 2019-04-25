// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "DefaultAllocator.hpp"
#include "ReadForwardingPolicy.hpp"
#include "StreamProxy.hpp"
#include "WriteBufferingPolicy.hpp"

namespace StreamUtils {

template <typename TAllocator>
struct BasicWriteBufferingStream
    : StreamProxy<ReadForwardingPolicy, WriteBufferingPolicy<TAllocator>> {
  explicit BasicWriteBufferingStream(Stream &upstream, size_t capacity,
                                     TAllocator allocator = TAllocator())
      : StreamProxy<ReadForwardingPolicy, WriteBufferingPolicy<TAllocator>>(
            upstream, {}, {capacity, allocator}) {}
};  // namespace StreamUtils

using WriteBufferingStream = BasicWriteBufferingStream<DefaultAllocator>;
}  // namespace StreamUtils