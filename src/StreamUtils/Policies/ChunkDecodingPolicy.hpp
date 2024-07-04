// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2024
// MIT License

#pragma once

#include <Client.h>
#include <assert.h>

namespace StreamUtils {

class ChunkDecodingPolicy {
  enum class State {
    ChunkSize,
    ChunkExtensions,
    ChunkStart,
    ChunkBody,
    ChunkEnd,
    TrailerStart,
    Trailer,
    TrailerEnd,
    FinalCrLf,
    Error,
  };

 public:
  int available(Stream &target) {
    if (!isInChunkBody(target))
      return 0;
    return min(target.available(), remaining_);
  }

  int read(Stream &target) {
    if (!isInChunkBody(target))
      return -1;
    int c = target.read();
    if (c >= 0)
      decreaseRemaining(1);
    return c;
  }

  int peek(Stream &target) {
    if (!isInChunkBody(target))
      return -1;
    return target.peek();
  }

  size_t readBytes(Stream &target, char *buffer, size_t size) {
    if (!isInChunkBody(target))
      return 0;
    size_t n = target.readBytes(buffer, min(size, remaining_));
    decreaseRemaining(n);
    return n;
  }

  int read(Client &target, uint8_t *buffer, size_t size) {
    return readBytes(target, reinterpret_cast<char *>(buffer), size);
  }

 private:
  bool isInChunkBody(Stream &target) {
    while (state_ != State::Error && state_ != State::ChunkBody &&
           target.available()) {
      state_ = injestNext(target);
    }
    return state_ == State::ChunkBody;
  }

  State injestNext(Stream &target) {
    int c = target.read();
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

      case State::ChunkEnd:
        if (c == '\r')
          return State::ChunkEnd;
        else if (c == '\n')
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
          return State::ChunkSize;  // Read for a new response
        } else
          return State::Error;

      default:
        assert(false);
        return State::Error;
    }
  }

  State appendSizeHexDigit(uint8_t digit) {
    remaining_ = remaining_ * 16 + digit;
    return State::ChunkSize;
  }

  void decreaseRemaining(size_t n) {
    assert(remaining_ >= n);
    remaining_ -= n;
    if (remaining_ == 0)
      state_ = State::ChunkEnd;
  }

  size_t min(size_t a, size_t b) {
    return a < b ? a : b;
  }

  size_t remaining_ = 0;
  State state_ = State::ChunkSize;
};

}  // namespace StreamUtils
