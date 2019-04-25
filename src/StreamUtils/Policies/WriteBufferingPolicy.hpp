// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "../Buffers/LinearBuffer.hpp"

namespace StreamUtils {

template <typename TAllocator>
struct WriteBufferingPolicy {
 public:
  WriteBufferingPolicy(size_t capacity, TAllocator allocator)
      : _buffer(capacity, allocator) {}

  size_t write(Stream &stream, const uint8_t *data, size_t size) {
    size_t result = 0;

    // continue to fill the buffer?
    if (!_buffer.isEmpty()) {
      size_t n = _buffer.write(data, size);
      data += n;
      size -= n;
      result += n;

      // time to flush?
      if (_buffer.isFull()) {
        _buffer.flushInto(stream);
      }
    }

    // something left to write?
    if (size > 0) {
      // can we bypass the buffer?
      if (size >= _buffer.capacity()) {
        result += stream.write(data, size);
      } else {
        result += _buffer.write(data, size);
      }
    }
    return result;
  }

  size_t write(Stream &stream, uint8_t data) {
    if (!_buffer)
      return stream.write(data);

    _buffer.write(data);
    if (_buffer.isFull())
      _buffer.flushInto(stream);
    return 1;
  }

  void flush(Stream &stream) {
    _buffer.flushInto(stream);
    stream.flush();
  }

  void detach(Stream &stream) {
    _buffer.flushInto(stream);
  }

 private:
  LinearBuffer<TAllocator> _buffer;
};

}  // namespace StreamUtils