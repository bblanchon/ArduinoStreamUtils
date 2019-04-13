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
    _begin = _end = _size = 0;
  }

  CircularBuffer(const CircularBuffer &src)
      : CircularBuffer(src._capacity, src._allocator) {
    if (_data != nullptr) {
      memcpy(_data, src._data, _capacity);
      _begin = src._begin;
      _end = src._end;
      _size = src._size;
    }
  }

  ~CircularBuffer() {
    _allocator.deallocate(_data);
  }

  char *data() const {
    return reinterpret_cast<char *>(_data);
  }

  size_t available() const {
    return _size;
  }

  size_t capacity() const {
    return _capacity;
  }

  void clear() {
    _begin = _end = _size = 0;
  }

  bool isEmpty() const {
    return _size == 0;
  }

  bool isFull() const {
    return _size == _capacity;
  }

  operator bool() const {
    return capacity() > 0;
  }

  char peek() const {
    assert(_size > 0);
    return _data[_begin];
  }

  char read() {
    assert(_size > 0);
    char result = _data[_begin];
    _begin = (_begin + 1) % _capacity;
    _size--;
    return result;
  }

  size_t readBytes(char *data, size_t size) {
    // don't read more that available
    if (size > _size)
      size = _size;

    for (size_t i = 0; i < size; i++)
      data[i] = read();

    return size;
  }

  size_t write(uint8_t data) {
    assert(_size < _capacity);
    _data[_end] = data;
    _end = (_end + 1) % _capacity;
    _size++;
    return 1;
  }

  size_t write(const uint8_t *data, size_t size) {
    // don't read more that available
    size_t roomLeft = _capacity - _size;
    if (size > roomLeft)
      size = roomLeft;

    for (size_t i = 0; i < size; i++)
      write(data[i]);

    return size;
  }

 private:
  TAllocator _allocator;
  char *_data;
  size_t _capacity;
  size_t _size;
  size_t _begin;
  size_t _end;
};  // namespace StreamUtils

}  // namespace StreamUtils