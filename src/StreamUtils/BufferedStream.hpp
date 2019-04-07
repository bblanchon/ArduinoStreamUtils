// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "DefaultAllocator.hpp"

namespace StreamUtils {

template <typename TAllocator>
class BasicStreamWithInputBuffer : public Stream {
 public:
  explicit BasicStreamWithInputBuffer(Stream &upstream, size_t capacity,
                                      TAllocator allocator = TAllocator())
      : _allocator(allocator),
        _upstream(upstream),
        _buffer(reinterpret_cast<char *>(allocator.allocate(capacity))),
        _capacity(capacity),
        _begin(_buffer),
        _end(_buffer) {}

  BasicStreamWithInputBuffer(const BasicStreamWithInputBuffer &other)
      : BasicStreamWithInputBuffer(other._upstream, other._capacity,
                                   other._allocator) {
    if (_buffer) {
      size_t n = other._end - other._begin;
      memcpy(_begin, other._begin, n);
      _end += n;
    }
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
    // TODO: if (_buffer == nullptr) return _upstream.readBytes(buffer, size);

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
      if (size < _capacity) {
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

 private:
  bool isEmpty() const {
    return _begin >= _end;
  }

  void reloadIfEmpty() {
    if (!isEmpty()) return;
    reload();
  }

  size_t reload() {
    // TODO: if (_buffer == nullptr) return 0;
    size_t bytesRead = _upstream.readBytes(_buffer, _capacity);
    _begin = _buffer;
    _end = _begin + bytesRead;
    return bytesRead;
  }

  TAllocator _allocator;
  Stream &_upstream;
  char *_buffer;
  size_t _capacity;
  char *_begin;
  char *_end;
};

using StreamWithInputBuffer = BasicStreamWithInputBuffer<DefaultAllocator>;

StreamWithInputBuffer bufferizeInput(Stream &upstream, size_t capacity) {
  return StreamWithInputBuffer(upstream, capacity);
}
}  // namespace StreamUtils