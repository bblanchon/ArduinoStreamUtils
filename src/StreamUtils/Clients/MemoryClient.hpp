// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include <Client.h>

#include "../Buffers/CircularBuffer.hpp"
#include "../Ports/DefaultAllocator.hpp"
#include "../Streams/MemoryStream.hpp"

namespace StreamUtils {

template <typename TAllocator>
class BasicMemoryClient : public Client {
 public:
  BasicMemoryClient(size_t capacity, TAllocator allocator = TAllocator())
      : _stream(capacity, allocator), _connected(false) {}

  BasicMemoryClient(const BasicMemoryClient &src) : _stream(src._stream) {}

  // --- Print ---

  size_t write(uint8_t data) {
    return _stream.write(data);
  }

  size_t write(const uint8_t *data, size_t size) override {
    return _stream.write(data, size);
  }

  // --- Stream ---

  int available() override {
    return _stream.available();
  }

  int peek() override {
    return _stream.peek();
  }

  int read() override {
    return _stream.read();
  }

  // WARNING: we cannot use "override" because most cores don't define this
  // function as virtual
  size_t readBytes(char *data, size_t size) override {
    return _stream.readBytes(data, size);
  }

  void flush() override {
    _stream.flush();
  }

  // --- Client ---

  int connect(IPAddress, uint16_t) override {
    _connected = true;
    return 1;
  }

  int connect(const char *, uint16_t) override {
    _connected = true;
    return 1;
  }

  uint8_t connected() override {
    return _connected;
  }

  void stop() override {
    _connected = false;
  }

  operator bool() override {
    return true;
  }

  int read(uint8_t *buf, size_t size) override {
    return _stream.readBytes(reinterpret_cast<char *>(buf), size);
  }

 private:
  BasicMemoryStream<TAllocator> _stream;
  bool _connected;
};
using MemoryClient = BasicMemoryClient<DefaultAllocator>;

}  // namespace StreamUtils