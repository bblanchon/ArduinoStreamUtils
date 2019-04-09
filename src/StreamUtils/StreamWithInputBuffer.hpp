// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "CircularBuffer.hpp"
#include "DefaultAllocator.hpp"

namespace StreamUtils {

template <typename TAllocator>
class BasicStreamWithInputBuffer : public Stream {
 public:
  explicit BasicStreamWithInputBuffer(Stream &upstream, size_t capacity,
                                      TAllocator allocator = TAllocator())
      : _upstream(upstream), _buffer(capacity, allocator) {}

  BasicStreamWithInputBuffer(const BasicStreamWithInputBuffer &other)
      : _upstream(other._upstream), _buffer(other._buffer) {}

  BasicStreamWithInputBuffer &operator=(const BasicStreamWithInputBuffer &) =
      delete;

  size_t write(const uint8_t *buffer, size_t size) override {
    return _upstream.write(buffer, size);
  }

  size_t write(uint8_t data) override {
    return _upstream.write(data);
  }

  int available() override {
    return _upstream.available() + _buffer.available();
  }

  int read() override {
    if (!_buffer) return _upstream.read();
    reloadIfEmpty();
    return isEmpty() ? -1 : _buffer.read();
  }

  int peek() override {
    return isEmpty() ? _upstream.peek() : _buffer.peek();
  }

  void flush() override {
    _upstream.flush();
  }

  // WARNING: we cannot use "override" because most cores don't define this
  // function as virtual
  virtual size_t readBytes(char *buffer, size_t size) {
    if (!_buffer) return _upstream.readBytes(buffer, size);

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
        reload();
        size_t bytesRead = _buffer.readBytes(buffer, size);
        result += bytesRead;
      } else {
        // we can bypass the buffer
        result += _upstream.readBytes(buffer, size);
      }
    }

    return result;
  }

  size_t capacity() const {
    return _buffer.capacity();
  }

 private:
  bool isEmpty() const {
    return _buffer.available() == 0;
  }

  void reloadIfEmpty() {
    if (!isEmpty()) return;
    reload();
  }

  void reload() {
    _buffer.reloadFrom(_upstream);
  }

  Stream &_upstream;
  CircularBuffer<TAllocator> _buffer;
};

using StreamWithInputBuffer = BasicStreamWithInputBuffer<DefaultAllocator>;

StreamWithInputBuffer bufferizeInput(Stream &upstream, size_t capacity) {
  return StreamWithInputBuffer(upstream, capacity);
}
}  // namespace StreamUtils