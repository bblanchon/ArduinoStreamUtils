// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

namespace StreamUtils {
// Adds logging to an instance of Stream
class StreamLogger : public Stream {
 public:
  // Constructs a StreamLogger attached to the specified target
  StreamLogger(Stream &target) : _target(target) {}

  // Pops the next byte from the stream
  int read() override {
    // Delegate to the target
    int c = _target.read();
    // Log if something was returned
    if (c > 0) Serial.print((char)c);
    // Pretend nothing happened
    return c;
  }

  // WARNING: we cannot use "override" because most cores don't define this
  // function as virtual
  virtual size_t readBytes(char *buffer, size_t size) {
    // Delegate to the target
    size_t result = _target.readBytes(buffer, size);
    // Log
    Serial.write((const uint8_t *)buffer, result);
    // Pretend nothing happened
    return result;
  }

  // Gets the number of bytes available
  int available() override {
    // Delegate to target
    return _target.available();
  }

  // Gets the next byte in the stream (but doesn't pop it)
  int peek() override {
    // Delegate to target
    return _target.peek();
  }

  // Writes a single byte
  size_t write(uint8_t c) override {
    // Delegate to target
    return _target.write(c);
  }

  // Writes multiple bytes
  size_t write(const uint8_t *buffer, size_t size) override {
    // Delegate to target
    return _target.write(buffer, size);
  }

  // Flushes temporary buffers
  void flush() override {
    // Delegate to target
    _target.flush();
  }

 private:
  Stream &_target;
};
}  // namespace StreamUtils