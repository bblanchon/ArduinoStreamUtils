// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2020
// MIT License

#pragma once

#include "../Policies/ReadThrottlingPolicy.hpp"
#include "../Policies/WriteForwardingPolicy.hpp"
#include "StreamProxy.hpp"

#ifdef ARDUINO
#include "../Ports/DefaultThrottler.hpp"
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