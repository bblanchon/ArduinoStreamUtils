// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "DefaultAllocator.hpp"
#include "LinearBuffer.hpp"

namespace StreamUtils {

template <typename TAllocator>
class BasicWriteBufferingStream : public Stream {
 public:
  explicit BasicWriteBufferingStream(Stream &upstream, size_t capacity,
                                     TAllocator allocator = TAllocator())
      : _upstream(upstream), _buffer(capacity, allocator) {}

  BasicWriteBufferingStream(const BasicWriteBufferingStream &other)
      : _upstream(other._upstream), _buffer(other._buffer) {}

  BasicWriteBufferingStream &operator=(const BasicWriteBufferingStream &) =
      delete;

  ~BasicWriteBufferingStream() {
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
    if (!_buffer)
      return _upstream.write(data);

    _buffer.write(data);
    if (_buffer.isFull())
      _buffer.flushInto(_upstream);
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
  LinearBuffer<TAllocator> _buffer;
};  // namespace StreamUtils

using WriteBufferingStream = BasicWriteBufferingStream<DefaultAllocator>;
}  // namespace StreamUtils