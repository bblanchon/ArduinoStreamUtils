// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2020
// MIT License

#pragma once

#include <Print.h>

namespace StreamUtils {

template <typename WritePolicy>
class PrintProxy : public Print {
 public:
  explicit PrintProxy(Print &upstream, WritePolicy writer)
      : _target(upstream), _writer(writer) {}

  PrintProxy(const PrintProxy &other)
      : _target(other._target), _writer(other._writer) {}

  ~PrintProxy() {
    _writer.implicitFlush(_target);
  }

  size_t write(const uint8_t *buffer, size_t size) override {
    return _writer.write(_target, buffer, size);
  }

  size_t write(uint8_t data) override {
    return _writer.write(_target, data);
  }

#if STREAMUTILS_PRINT_FLUSH_EXISTS
  void flush() override {
#else
  void flush() {
#endif
    _writer.flush(_target);
  }

  using Print::write;

 private:
  Print &_target;
  WritePolicy _writer;
};

}  // namespace StreamUtils