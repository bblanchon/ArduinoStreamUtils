// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

namespace StreamUtils {

class BufferedStream : public Stream {
 public:
  explicit BufferedStream(Stream &upstream, char *buffer, size_t capacity)
      : _upstream(upstream),
        _buffer(buffer),
        _capacity(capacity),
        _begin(buffer),
        _end(buffer) {}

  BufferedStream(const BufferedStream &other, char *buffer, size_t capacity)
      : _upstream(other._upstream),
        _buffer(buffer),
        _capacity(capacity),
        _begin(buffer + (other._begin - other._buffer)),
        _end(buffer + (other._end - other._buffer)) {
    memcpy(_begin, other._begin, _end - _begin);
  }

  BufferedStream(const BufferedStream &) = delete;
  BufferedStream &operator=(const BufferedStream &) = delete;

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
    size_t bytesRead = _upstream.readBytes(_buffer, _capacity);
    _begin = _buffer;
    _end = _begin + bytesRead;
    return bytesRead;
  }

  Stream &_upstream;
  char *_buffer;
  size_t _capacity;
  char *_begin;
  char *_end;
};

template <size_t capacity>
class StaticBufferedStream : public BufferedStream {
 public:
  explicit StaticBufferedStream(Stream &upstream)
      : BufferedStream(upstream, _buffer, capacity) {}

  StaticBufferedStream(const StaticBufferedStream &other)
      : BufferedStream(other, _buffer, capacity) {}

 private:
  char _buffer[capacity];
};

template <typename Stream>
StaticBufferedStream<64> bufferizeInput(Stream &upstream) {
  return StaticBufferedStream<64>{upstream};
}

}  // namespace StreamUtils