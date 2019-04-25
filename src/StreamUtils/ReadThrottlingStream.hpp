// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "ReadThrottlingPolicy.hpp"
#include "StreamProxy.hpp"
#include "WriteForwardingPolicy.hpp"

#ifdef ARDUINO
#include "DefaultThrottler.hpp"
#endif

namespace StreamUtils {

template <typename TThrottler>
class BasicReadThrottlingStream
    : public StreamProxy<ReadThrottlingPolicy<TThrottler>,
                         WriteForwardingPolicy> {
 public:
  BasicReadThrottlingStream(Stream &upstream,
                            TThrottler throttler = TThrottler())
      : StreamProxy<ReadThrottlingPolicy<TThrottler>, WriteForwardingPolicy>(
            upstream, {}, {throttler}) {}
};

#ifdef ARDUINO
using ReadThrottlingStream = BasicReadThrottlingStream<DefaultThrottler>;
#endif
}  // namespace StreamUtils