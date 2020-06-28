// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2020
// MIT License

#pragma once

#include <Stream.h>

#include "../Buffers/LinearBuffer.hpp"
#include "../Helpers.hpp"

namespace StreamUtils {

template <typename TAllocator>
struct ReadBufferingPolicy {
  ReadBufferingPolicy(size_t capacity, TAllocator allocator = TAllocator())
      : _buffer(capacity, allocator) {}

  ReadBufferingPolicy(const ReadBufferingPolicy &other)
      : _buffer(other._buffer) {}

  int available(Stream &stream) {
    return stream.available() + _buffer.available();
  }

  template <typename TTarget>  // Stream or Client
  int read(TTarget &target) {
    if (!_buffer)
      return target.read();
    reloadIfEmpty(target);
    return isEmpty() ? -1 : _buffer.read();
  }

  int peek(Stream &stream) {
    return isEmpty() ? stream.peek() : _buffer.peek();
  }

  template <typename TTarget>  // Stream or Client
  size_t readBytes(TTarget &target, char *buffer, size_t size) {
    if (!_buffer)
      return optimizedRead(target, buffer, size);

    size_t result = 0;

    // can we read from buffer?
    if (_buffer.available() > 0) {
      size_t bytesRead = _buffer.readBytes(buffer, size);
      result += bytesRead;
      buffer += bytesRead;
      size -= bytesRead;
    }

    // still something to read?
    if (size > 0) {
      // (at this point, the buffer is empty)

      // should we use the buffer?
      if (size < _buffer.capacity()) {
        reload(target);
        size_t bytesRead = _buffer.readBytes(buffer, size);
        result += bytesRead;
      } else {
        // we can bypass the buffer
        result += optimizedRead(target, buffer, size);
      }
    }

    return result;
  }

  size_t read(Client &client, uint8_t *buffer, size_t size) {
    return readBytes(client, reinterpret_cast<char *>(buffer), size);
  }

 private:
  bool isEmpty() const {
    return _buffer.available() == 0;
  }

  template <typename TTarget>  // Stream or Client
  void reloadIfEmpty(TTarget &target) {
    if (!isEmpty())
      return;
    reload(target);
  }

  template <typename TTarget>  // Stream or Client
  void reload(TTarget &target) {
    _buffer.reloadFrom(target);
  }

  LinearBuffer<TAllocator> _buffer;
};

}  // namespace StreamUtils