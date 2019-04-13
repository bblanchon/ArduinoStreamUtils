// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "CircularBuffer.hpp"
#include "DefaultAllocator.hpp"

namespace StreamUtils {

template <typename TAllocator>
class BasicStreamWithOutputBuffer : public Stream {
 public:
  explicit BasicStreamWithOutputBuffer(Stream &upstream, size_t capacity,
                                       TAllocator allocator = TAllocator())
      : _upstream(upstream), _buffer(capacity, allocator) {}

  BasicStreamWithOutputBuffer(const BasicStreamWithOutputBuffer &other)
      : _upstream(other._upstream), _buffer(other._buffer) {}

  BasicStreamWithOutputBuffer &operator=(const BasicStreamWithOutputBuffer &) =
      delete;

  ~BasicStreamWithOutputBuffer() {
    _buffer.flushInto(_upstream);
  }

  using Stream::write;

  size_t write(const uint8_t *data, size_t size) override {
    size_t result = 0;

    // continue to fill the buffer?
    if (!_buffer.isEmpty()) {
      size_t n = _buffer.write(data, size);
      data += n;
      size -= n;
      result += n;

      // time to flush?
      if (_buffer.isFull()) {
        _buffer.flushInto(_upstream);
      }
    }

    // something left to write?
    if (size > 0) {
      // can we bypass the buffer?
      if (size >= capacity()) {
        result += _upstream.write(data, size);
      } else {
        result += _buffer.write(data, size);
      }
    }
    return result;
  }

  size_t write(uint8_t data) override {
    if (!_buffer) return _upstream.write(data);

    _buffer.write(data);
    if (_buffer.isFull()) _buffer.flushInto(_upstream);
    return 1;
  }

  int available() override {
    return _upstream.available();
  }

  int read() override {
    return _upstream.read();
  }

  int peek() override {
    return _upstream.peek();
  }

  void flush() override {
    _buffer.flushInto(_upstream);
    _upstream.flush();
  }

  // WARNING: we cannot use "override" because most cores don't define this
  // function as virtual
  virtual size_t readBytes(char *buffer, size_t size) {
    return _upstream.readBytes(buffer, size);
  }

  size_t capacity() const {
    return _buffer.capacity();
  }

 private:
  Stream &_upstream;
  CircularBuffer<TAllocator> _buffer;
};  // namespace StreamUtils

using StreamWithOutputBuffer = BasicStreamWithOutputBuffer<DefaultAllocator>;

StreamWithOutputBuffer bufferOutput(Stream &upstream, size_t capacity) {
  return StreamWithOutputBuffer(upstream, capacity);
}
}  // namespace StreamUtils