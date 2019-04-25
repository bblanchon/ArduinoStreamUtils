// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "LinearBuffer.hpp"

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

  int read(Stream &stream) {
    if (!_buffer)
      return stream.read();
    reloadIfEmpty(stream);
    return isEmpty() ? -1 : _buffer.read();
  }

  int peek(Stream &stream) {
    return isEmpty() ? stream.peek() : _buffer.peek();
  }

  // WARNING: we cannot use "override" because most cores don't define this
  // function as virtual
  size_t readBytes(Stream &stream, char *buffer, size_t size) {
    if (!_buffer)
      return stream.readBytes(buffer, size);

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
        reload(stream);
        size_t bytesRead = _buffer.readBytes(buffer, size);
        result += bytesRead;
      } else {
        // we can bypass the buffer
        result += stream.readBytes(buffer, size);
      }
    }

    return result;
  }

 private:
  bool isEmpty() const {
    return _buffer.available() == 0;
  }

  void reloadIfEmpty(Stream &stream) {
    if (!isEmpty())
      return;
    reload(stream);
  }

  void reload(Stream &stream) {
    _buffer.reloadFrom(stream);
  }

  LinearBuffer<TAllocator> _buffer;
};

}  // namespace StreamUtils