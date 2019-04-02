// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2019
// MIT License

#include "Stream.hpp"
#include "StreamSpy.hpp"
#include "StreamStub.hpp"

#include "StreamUtils/BufferedStream.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <sstream>
#include <string>

using namespace StreamUtils;

TEST_CASE("BufferedStream") {
  StreamStub stub;
  StreamSpy spy{stub};
  BufferedStream<4> bufferedStream{spy};
  Stream& stream = bufferedStream;

  SUBCASE("available()") {
    stub.setup("ABCDEFGH");

    SUBCASE("same a upstream") {
      CHECK(stream.available() == 8);
      CHECK(spy.log() == "available() -> 8");
    }

    SUBCASE("upstream + in buffer") {
      stream.read();

      CHECK(stream.available() == 7);
      CHECK(spy.log() ==
            "readBytes(4) -> 4"
            "available() -> 4");
    }
  }

  SUBCASE("peek()") {
    SUBCASE("returns -1 when empty") {
      stub.setup("");

      int result = stream.peek();

      CHECK(result == -1);
      CHECK(spy.log() == "peek() -> -1");
    }

    SUBCASE("doesn't call readBytes() when buffer is empty") {
      stub.setup("A");

      int result = stream.peek();

      CHECK(result == 'A');
      CHECK(spy.log() == "peek() -> 65");
    }

    SUBCASE("doesn't call peek() when buffer is full") {
      stub.setup("AB");

      stream.read();
      int result = stream.peek();

      CHECK(result == 'B');
      CHECK(spy.log() == "readBytes(4) -> 2");
    }
  }

  SUBCASE("read()") {
    SUBCASE("reads 4 bytes at a time") {
      stub.setup("ABCDEFG");
      std::string result;

      for (int i = 0; i < 7; i++) {
        result += (char)stream.read();
      }

      CHECK(result == "ABCDEFG");
      CHECK(spy.log() ==
            "readBytes(4) -> 4"
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