// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#ifdef ARDUINO
#include "DefaultThrottler.hpp"
#endif

namespace StreamUtils {

template <typename TThrottler>
class BasicReadThrottlingStream : public Stream {
 public:
  BasicReadThrottlingStream(Stream &upstream,
                            TThrottler throttler = TThrottler())
      : _upstream(upstream), _throttler(throttler) {}

  int available() override {
    return _upstream.available();
  }

  void flush() override {
    _upstream.flush();
  }

  int peek() override {
    _throttler.throttle();
    return _upstream.peek();
  }

  int read() override {
    _throttler.throttle();
    return _upstream.read();
  }

  // WARNING: we cannot use "override" because most cores don't define this
  // function as virtual
  virtual size_t readBytes(char *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
      int c = read();
      if (c < 0)
        return i;
      buffer[i] = c;
    }
    return size;
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
  TThrottler _throttler;
};

#ifdef ARDUINO
using ReadThrottlingStream = BasicReadThrottlingStream<DefaultThrottler>;
#endif
}  // namespace StreamUtils