// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2019
// MIT License

#include "Stream.hpp"

#include "StreamUtils/BufferedStream.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <sstream>
#include <string>

using namespace StreamUtils;

class StreamTestSpy {
public:
  virtual size_t write(const uint8_t *buffer, size_t size) {
    _log << "write(" << size << ")";
    return size;
  }
  virtual size_t write(uint8_t data) {
    _log << "write()";
    return 1;
  }
  virtual int available() {
    _log << "available()";
    return true;
  }
  virtual int read() {
    _log << "read()";
    return 42;
  }
  virtual int peek() {
    _log << "peek()";
    return 42;
  }
  virtual void flush() { _log << "flush()"; }

  size_t readBytes(char *buffer, size_t length) {
    _log << "read(" << length << ")";
    return length;
  }

  std::string log() const { return _log.str(); }

private:
  std::stringstream _log;
};

TEST_CASE("BufferedStream") {
  StreamTestSpy spy;
  auto stream = bufferizeInput(spy);

  SUBCASE("read()") {
    stream.read();
    CHECK(spy.log() == "read()");
  }

  SUBCASE("flush()") {
    stream.flush();
    CHECK(spy.log() == "flush()");
  }
}