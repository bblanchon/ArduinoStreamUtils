// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2024
// MIT License

#pragma once

#include <Client.h>
#include <assert.h>
#include "../Helpers.hpp"

namespace StreamUtils {

class ChunkDecodingPolicy {
  enum class State {
    ChunkSize,
    ChunkExtensions,
    ChunkStart,
    ChunkBody,
    ChunkEndCr,
    ChunkEndLf,
    TrailerStart,
    Trailer,
    TrailerEnd,
    FinalCrLf,
    Ended,
    Error,
  };

 public:
  int available(Stream &target) {
    if (!goToChunkBody(target))
      return 0;
    return min_(target.available(), remaining_);
  }

  int read(Stream &target) {
    if (!goToChunkBody(target))
      return -1;
    int c = target.read();
    if (c >= 0)
      decreaseRemaining(1);
    return c;
  }

  int peek(Stream &target) {
    if (!goToChunkBody(target))
      return -1;
    return target.peek();
  }

  size_t readBytes(Stream &target, char *buffer, size_t size) {
    return doReadBytes(target, buffer, size);
  }

  int read(Client &target, uint8_t *buffer, size_t size) {
    return static_cast<int>(
        doReadBytes(target, reinterpret_cast<char *>(buffer), size));
  }

  bool error() const {
    return state_ == State::Error;
  }

  bool ended() const {
    return state_ == State::Ended;
  }

 private:
  template <typename TTarget>  // Stream or Client
  size_t doReadBytes(TTarget &target, char *buffer, size_t size) {
    size_t result = 0;
    while (size > 0 && !error() && !ended() && goToChunkBody(target, true)) {
      size_t n = readOrReadBytes(target, buffer, min_(size, remaining_));
      decreaseRemaining(n);
      result += n;
      size -= n;
      buffer += n;
    }
    return result;
  }

  bool inBody() const {
    return state_ == State::ChunkBody;
  }

  template <typename TTarget>  // Stream or Client
  bool goToChunkBody(TTarget &target, bool wait = false) {
    while (!error() && !ended() && !inBody()) {
      int c = readNextChar(target, wait);
      if (c < 0)
        return false;
      state_ = interpret(static_cast<char>(c));
    }
    return state_ == State::ChunkBody;
  }

  template <typename TTarget>  // Stream or Client
  int readNextChar(TTarget &target, bool wait = false) {
    if (wait) {
      char c;
      if (readOrReadBytes(target, &c, 1) == 1)
        return c;
      else
        return -1;
    } else {
      return target.read();
    }
  }

  State interpret(char c) {
    switch (state_) {
      case State::ChunkSize:
        if (c >= '0' && c <= '9')
          return appendSizeHexDigit(c - '0');
        else if (c >= 'A' && c <= 'F')
          return appendSizeHexDigit(c - 'A' + 10);
        else if (c >= 'a' && c <= 'f')
          return appendSizeHexDigit(c - 'a' + 10);
        else if (c == '\r')
          return State::ChunkStart;
        else if (c == ' ' || c == '\t' || c == ';')
          return State::ChunkExtensions;
        else
          return State::Error;

      case State::ChunkExtensions:
        if (c == '\r')
          return State::ChunkStart;
        else
          return State::ChunkExtensions;

      case State::ChunkStart:
        if (c == '\n') {
          if (remaining_ == 0)
            return State::TrailerStart;
          else
            return State::ChunkBody;
        } else
          return State::Error;

      case State::ChunkEndCr:
        if (c == '\r')
          return State::ChunkEndLf;
        else
          return State::Error;

      case State::ChunkEndLf:
        if (c == '\n')
          return State::ChunkSize;
        else
          return State::Error;

      case State::TrailerStart:
        if (c == '\r')
          return State::FinalCrLf;
        else
          return State::Trailer;

      case State::Trailer:
        if (c == '\r')
          return State::TrailerEnd;
        else if (c == '\n')
          return State::Error;
        else
          return State::Trailer;

      case State::TrailerEnd:
        if (c == '\n')
          return State::TrailerStart;
        else
          return State::Error;

      case State::FinalCrLf:
        if (c == '\n') {
          assert(remaining_ == 0);
          return State::Ended;
        } else
          return State::Error;

      default:
        return State::Error;
    }
  }

  State appendSizeHexDigit(uint8_t digit) {
    auto oldRemaining = remaining_;
    remaining_ = oldRemaining * 16 + digit;
    if (remaining_ < oldRemaining) {  // overflow
      remaining_ = 0;
      return State::Error;
    }
    return State::ChunkSize;
  }

  void decreaseRemaining(size_t n) {
    assert(remaining_ >= n);
    remaining_ -= n;
    if (remaining_ == 0)
      state_ = State::ChunkEndCr;
  }

  size_t min_(size_t a, size_t b) {
    return a < b ? a : b;
  }

  size_t remaining_ = 0;
  State state_ = State::ChunkSize;
};

}  // namespace StreamUtils
