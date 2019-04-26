// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

namespace StreamUtils {

template <typename WritePolicy>
class PrintProxy : public Print {
 public:
  explicit PrintProxy(Print &upstream, WritePolicy writer)
      : _upstream(upstream), _writer(writer) {}

  PrintProxy(const PrintProxy &other)
      : _upstream(other._upstream), _writer(other._writer) {}

  size_t write(const uint8_t *buffer, size_t size) override {
    return _writer.write(_upstream, buffer, size);
  }

  size_t write(uint8_t data) override {
    return _writer.write(_upstream, data);
  }

  using Print::write;

 private:
  Print &_upstream;
  WritePolicy _writer;
};

}  // namespace StreamUtils