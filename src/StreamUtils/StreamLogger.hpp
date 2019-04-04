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
  virtual int read() {
    // Delegate to the target
    int c = _target.read();
    // Log if something was returned
    if (c > 0) Serial.print((char)c);
    // Pretend nothing happened
    return c;
  }

  // Gets the number of bytes available
  virtual int available() {
    // Delegate to target
    return _target.available();
  }

  // Gets the next byte in the stream (but doesn't pop it)
  virtual int peek() {
    // Delegate to target
    return _target.peek();
  }

  // Writes a single byte
  virtual size_t write(uint8_t c) {
    // Delegate to target
    return _target.write(c);
  }

  // Writes multiple bytes
  virtual size_t write(const uint8_t *buffer, size_t size) {
    // Delegate to target
    return _target.write(buffer, size);
  }

  // Flushes temporary buffers
  virtual void flush() {
    // Delegate to target
    _target.flush();
  }

 private:
  Stream &_target;
};
}  // namespace StreamUtils