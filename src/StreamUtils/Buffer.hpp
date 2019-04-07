// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include <stdlib.h>  // size_t, memcpy

namespace StreamUtils {

template <typename TAllocator>
class Buffer {
 public:
  Buffer(size_t capacity, TAllocator allocator = TAllocator())
      : _allocator(allocator) {
    _data = allocator.allocate(capacity);
    _capacity = _data ? capacity : 0;
  }

  ~Buffer() {
    _allocator.deallocate(_data);
  }

  char *data() const {
    return reinterpret_cast<char *>(_data);
  }

  size_t capacity() const {
    return _capacity;
  }

  size_t copyFrom(const char *begin, const char *end) {
    size_t n = end - begin;
    if (n > _capacity) n = _capacity;
    memcpy(_data, begin, n);
    return n;
  }

  operator bool() const {
    return _data != nullptr;
  }

 private:
  TAllocator _allocator;
  void *_data;
  size_t _capacity;
};

}  // namespace StreamUtils