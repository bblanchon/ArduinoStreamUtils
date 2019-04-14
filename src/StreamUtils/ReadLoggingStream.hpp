// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

namespace StreamUtils {

class ReadLoggingStream : public Stream {
 public:
  ReadLoggingStream(Stream &upstream, Stream &log)
      : _upstream(upstream), _log(log) {}

  int available() override {
    return _upstream.available();
  }

  void flush() override {
    _upstream.flush();
    _log.flush();
  }

  int peek() override {
    return _upstream.peek();
  }

  int read() override {
    int result = _upstream.read();
    if (result >= 0)
      _log.write(result);
    return result;
  }

  // WARNING: we cannot use "override" because most cores don't define this
  // function as virtual
  virtual size_t readBytes(char *buffer, size_t size) {
    size_t result = _upstream.readBytes(buffer, size);
    _log.write(buffer, result);
    return result;
  }

  size_t write(uint8_t c) override {
    return _upstream.write(c);
  }

  size_t write(const uint8_t *buffer, size_t size) override {
    return _upstream.write(buffer, size);
  }

  using Stream::write;

 private:
  Stream &_upstream;
  Stream &_log;
};
}  // namespace StreamUtils