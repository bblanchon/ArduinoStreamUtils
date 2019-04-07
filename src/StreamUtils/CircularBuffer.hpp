// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include <assert.h>
#include <stdlib.h>  // size_t, memcpy

namespace StreamUtils {

template <typename TAllocator>
class CircularBuffer {
 public:
  CircularBuffer(size_t capacity, TAllocator allocator = TAllocator())
      : _allocator(allocator) {
    _data = reinterpret_cast<char *>(_allocator.allocate(capacity));
    _capacity = _data ? capacity : 0;
    _begin = _data ? _data : nullptr;
    _end = _begin;
  }

  CircularBuffer(const CircularBuffer &src) : _allocator(src._allocator) {
    _data = reinterpret_cast<char *>(_allocator.allocate(src._capacity));
    if (_data != nullptr) {
      _capacity = src._capacity;
      memcpy(_data, src._begin, src.size());
      _begin = _data;
      _end = _data + _capacity;
    } else {
      _capacity = 0;
      _begin = nullptr;
      _end = nullptr;
    }
  }

  ~CircularBuffer() {
    _allocator.deallocate(_data);
  }

  char *data() const {
    return reinterpret_cast<char *>(_data);
  }

  size_t size() const {
    return _end - _begin;
  }

  size_t capacity() const {
    return _capacity;
  }

  operator bool() const {
    return _data != nullptr;
  }

  char peek() const {
    return *_begin;
  }

  char read() {
    return *_begin++;
  }

  size_t readBytes(char *dstPtr, size_t dstSize) {
    size_t srcSize = size();
    size_t n = srcSize < dstSize ? srcSize : dstSize;
    memcpy(dstPtr, _begin, n);
    _begin += n;
    return n;
  }

  template <typename LoadFunction>
  void reload(LoadFunction &&load) {
    size_t n = load(_data, _capacity);
    _begin = _data;
    _end = _data + n;
  }

 private:
  TAllocator _allocator;
  size_t _capacity;
  char *_data;
  char *_begin;
  char *_end;
};

}  // namespace StreamUtils