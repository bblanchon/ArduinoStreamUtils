// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

namespace StreamUtils {

class StreamOutputLogger : public Stream {
 public:
  StreamOutputLogger(Stream &upstream, Stream &log)
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
    return _upstream.read();
  }

  // WARNING: we cannot use "override" because most cores don't define this
  // function as virtual
  virtual size_t readBytes(char *buffer, size_t size) {
    return _upstream.readBytes(buffer, size);
  }

  size_t write(uint8_t c) override {
    size_t result = _upstream.write(c);
    _log.write(c);
    return result;
  }

  size_t write(const uint8_t *buffer, size_t size) override {
    size_t result = _upstream.write(buffer, size);
    _log.write(buffer, result);
    return result;
  }

  using Stream::write;

 private:
  Stream &_upstream;
  Stream &_log;
};
}  // namespace StreamUtils