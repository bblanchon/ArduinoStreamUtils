// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2024
// MIT License

#pragma once

#include "StreamUtils/Policies/ChunkDecodingPolicy.hpp"
#include "StreamUtils/Policies/ConnectForwardingPolicy.hpp"
#include "StreamUtils/Policies/WriteForwardingPolicy.hpp"

#include "ClientProxy.hpp"

namespace StreamUtils {

struct ChunkDecodingClient
    : ClientProxy<ChunkDecodingPolicy, WriteForwardingPolicy,
                  ConnectForwardingPolicy> {
  ChunkDecodingClient(Client &target)
      : ClientProxy<ChunkDecodingPolicy, WriteForwardingPolicy,
                    ConnectForwardingPolicy>(target) {}

  bool error() const {
    return _reader.error();
  }

  bool ended() const {
    return _reader.ended();
  }
};

}  // namespace StreamUtils
