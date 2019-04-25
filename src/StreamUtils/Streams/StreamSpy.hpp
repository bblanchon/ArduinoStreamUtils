// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

class StreamSpy : public Stream {
 public:
  StreamSpy(Stream &upstream, Stream &log) : _upstream(upstream), _log(log) {}

  virtual size_t write(const uint8_t *buffer, size_t size) {
    size_t result = _upstream.write(buffer, size);
    _log.print("write('");
    for (size_t i = 0; i < size; i++) {
      _log.write(buffer[i]);
    }
    _log.print("', ");
    _log.print(size);
    _log.print(") -> ");
    _log.print(result);
    return result;
  }

  virtual size_t write(uint8_t data) {
    size_t result = _upstream.write(data);
    _log.print("write('");
    _log.write(data);
    _log.print("') -> ");
    _log.print(result);
    return result;
  }

  virtual int available() {
    int result = _upstream.available();
    _log.print("available() -> ");
    _log.print(result);
    return result;
  }

  virtual int read() {
    int result = _upstream.read();
    _log.print("read() -> ");
    _log.print(result);
    return result;
  }

  virtual int peek() {
    int result = _upstream.peek();
    _log.print("peek() -> ");
    _log.print(result);
    return result;
  }

  virtual void flush() {
    _log.print("flush()");
    _upstream.flush();
  }

  size_t readBytes(char *buffer, size_t size) {
    size_t result = _upstream.readBytes(buffer, size);
    _log.print("readBytes(");
    _log.print(size);
    _log.print(") -> ");
    _log.print(result);
    return result;
  }

 private:
  Stream &_upstream;
  Stream &_log;
};
