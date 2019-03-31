// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2019
// MIT License

#include "Stream.hpp"
#include "StreamSpy.hpp"

#include "StreamUtils/BufferedStream.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <sstream>
#include <string>

using namespace StreamUtils;

class StreamStub {
public:
  void setup(const std::string &s) {
    _data.str(s);
    _available = s.length();
  }

  virtual size_t write(const uint8_t *, size_t size) { return size; }
  virtual size_t write(uint8_t) { return 1; }
  virtual int available() { return _available; }
  virtual int read() {
    _available--;
    return _data.get();
  }
  virtual int peek() { return _data.peek(); }
  virtual void flush() {}

  size_t readBytes(char *buffer, size_t size) {
    _data.read(buffer, size);
    _available -= size;
    return _data.gcount();
  }

private:
  std::stringstream _data;
  int _available;
};

template <typename TStream>
std::string callReadSeveralTimes(TStream stream, int times) {
  std::string result;
  for (int i = 0; i < times; i++)
    result += (char)stream.read();
  return result;
}

TEST_CASE("BufferedStream") {
  StreamStub stub;
  StreamSpy<StreamStub> spy{stub};
  BufferedStream<StreamSpy<StreamStub>, 4> stream{spy};

  SUBCASE("available()") {
    stub.setup("ABCDEFGH");

    SUBCASE("same a upstream") {
      CHECK(stream.available() == 8);
      CHECK(spy.log() == "available() -> 8");
    }

    SUBCASE("upstream + in buffer") {
      stream.read();

      CHECK(stream.available() == 7);
      CHECK(spy.log() == "readBytes(4) -> 4"
                         "available() -> 4");
    }
  }

  SUBCASE("read()") {

    SUBCASE("reads 4 bytes at a time") {
      stub.setup("ABCDEFG");
      std::string result = callReadSeveralTimes(stream, 7);

      CHECK(result == "ABCDEFG");
      CHECK(spy.log() == "readBytes(4) -> 4"
                         "readBytes(4) -> 3");
    }

    SUBCASE("returns -1 when empty") {
      stub.setup("");

      int result = stream.read();

      CHECK(result == -1);
      CHECK(spy.log() == "readBytes(4) -> 0");
    }
  }

  SUBCASE("flush()") {
    stream.flush();
    CHECK(spy.log() == "flush()");
  }
}