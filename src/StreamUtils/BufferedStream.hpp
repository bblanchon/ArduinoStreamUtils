// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

namespace StreamUtils {

template <size_t capacity>
class BufferedStream : public Stream {
 public:
  explicit BufferedStream(Stream &upstream)
      : _upstream(upstream), _begin(_buffer), _end(_buffer) {}

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
  virtual size_t readBytes(char *buffer, size_t length) {
    return _upstream.readBytes(buffer, length);
  }

 private:
  bool isEmpty() const {
    return _begin >= _end;
  }

  void reloadIfEmpty() {
    if (!isEmpty()) return;
    _begin = _buffer;
    _end = _begin + _upstream.readBytes(_buffer, capacity);
  }

  Stream &_upstream;
  char _buffer[capacity];
  char *_begin;
  char *_end;
};

template <typename Stream>
BufferedStream<64> bufferizeInput(Stream &upstream) {
  return BufferedStream<64>{upstream};
}

}  // namespace StreamUtils