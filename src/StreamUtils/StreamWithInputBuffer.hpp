// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "Buffer.hpp"
#include "DefaultAllocator.hpp"

namespace StreamUtils {

template <typename TAllocator>
class BasicStreamWithInputBuffer : public Stream {
 public:
  explicit BasicStreamWithInputBuffer(Stream &upstream, size_t capacity,
                                      TAllocator allocator = TAllocator())
      : _upstream(upstream),
        _buffer(capacity, allocator),
        _begin(_buffer.data()),
        _end(_buffer.data()) {}

  BasicStreamWithInputBuffer(const BasicStreamWithInputBuffer &other)
      : _upstream(other._upstream), _buffer(other._buffer) {
    _begin = _buffer.data() + (other._begin - other._buffer.data());
    _end = _buffer.data() + (other._end - other._buffer.data());
  }

  BasicStreamWithInputBuffer &operator=(const BasicStreamWithInputBuffer &) =
      delete;

  size_t write(const uint8_t *buffer, size_t size) override {
    return _upstream.write(buffer, size);
  }

  size_t write(uint8_t data) override {
    return _upstream.write(data);
  }

  int available() override {
    return _upstream.available() + _end - _begin;
  }

  int read() override {
    if (!_buffer) return _upstream.read();
    reloadIfEmpty();
    return isEmpty() ? -1 : *_begin++;
  }

  int peek() override {
    return isEmpty() ? _upstream.peek() : *_begin;
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
    if (!isEmpty()) {
      size_t bytesInBuffer = _end - _begin;
      size_t bytesToCopy = size < bytesInBuffer ? size : bytesInBuffer;
      memcpy(buffer, _begin, bytesToCopy);
      _begin += bytesToCopy;
      result += bytesToCopy;
      buffer += bytesToCopy;
      size -= bytesToCopy;
    }

    // still something to read?
    if (size > 0) {
      // (at this point, the buffer is empty)

      // should we use the buffer?
      if (size < _buffer.capacity()) {
        size_t bytesInBuffer = reload();
        size_t bytesToCopy = size < bytesInBuffer ? size : bytesInBuffer;
        memcpy(buffer, _begin, bytesToCopy);
        _begin += bytesToCopy;
        result += bytesToCopy;
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
    return _begin >= _end;
  }

  void reloadIfEmpty() {
    if (!isEmpty()) return;
    reload();
  }

  size_t reload() {
    size_t bytesRead = _upstream.readBytes(_buffer.data(), _buffer.capacity());
    _begin = _buffer.data();
    _end = _begin + bytesRead;
    return bytesRead;
  }

  Stream &_upstream;
  Buffer<TAllocator> _buffer;
  char *_begin;
  char *_end;
};

using StreamWithInputBuffer = BasicStreamWithInputBuffer<DefaultAllocator>;

StreamWithInputBuffer bufferizeInput(Stream &upstream, size_t capacity) {
  return StreamWithInputBuffer(upstream, capacity);
}
}  // namespace StreamUtils