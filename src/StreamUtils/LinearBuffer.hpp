// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include <assert.h>
#include <stdlib.h>  // size_t, memcpy

namespace StreamUtils {

template <typename TAllocator>
class LinearBuffer {
 public:
  LinearBuffer(size_t capacity, TAllocator allocator = TAllocator())
      : _allocator(allocator) {
    _data = reinterpret_cast<char *>(_allocator.allocate(capacity));
    _capacity = _data ? capacity : 0;
    _begin = _data ? _data : nullptr;
    _end = _begin;
  }

  LinearBuffer(const LinearBuffer &src) : _allocator(src._allocator) {
    _data = reinterpret_cast<char *>(_allocator.allocate(src._capacity));
    if (_data != nullptr) {
      _capacity = src._capacity;
      memcpy(_data, src._begin, src.available());
      _begin = _data;
      _end = _data + _capacity;
    } else {
      _capacity = 0;
      _begin = nullptr;
      _end = nullptr;
    }
  }

  ~LinearBuffer() {
    _allocator.deallocate(_data);
  }

  char *data() const {
    return reinterpret_cast<char *>(_data);
  }

  size_t available() const {
    return _end - _begin;
  }

  size_t capacity() const {
    return _capacity;
  }

  void clear() {
    _begin = _end = _data;
  }

  bool isEmpty() const {
    return available() == 0;
  }

  bool isFull() const {
    return available() == capacity();
  }

  operator bool() const {
    return _capacity > 0;
  }

  char peek() const {
    return *_begin;
  }

  char read() {
    return *_begin++;
  }

  size_t readBytes(char *dstPtr, size_t dstSize) {
    size_t srcSize = available();
    size_t n = srcSize < dstSize ? srcSize : dstSize;
    memcpy(dstPtr, _begin, n);
    _begin += n;
    return n;
  }

  size_t write(uint8_t data) {
    assert(!isFull());
    *_end++ = data;
    return 1;
  }

  size_t write(const uint8_t *data, size_t size) {
    size_t roomLeft = capacity() - available();
    if (size > roomLeft)
      size = roomLeft;
    memcpy(_end, data, size);
    _end += size;
    return size;
  }

  void reloadFrom(Stream &source) {
    size_t n = source.readBytes(_data, _capacity);
    _begin = _data;
    _end = _data + n;
  }

  void flushInto(Print &destination) {
    if (_begin != _end)
      destination.write(_begin, _end - _begin);
    _begin = _end = _data;
  }

 private:
  TAllocator _allocator;
  size_t _capacity;
  char *_data;
  char *_begin;
  char *_end;
};

}  // namespace StreamUtils