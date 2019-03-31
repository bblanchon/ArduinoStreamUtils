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

template <typename TStream>
std::string callReadSeveralTimes(TStream stream, int times) {
  std::string result;
  for (int i = 0; i < times; i++) result += (char)stream.read();
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
      CHECK(spy.log() ==
            "readBytes(4) -> 4"
            "available() -> 4");
    }
  }

  SUBCASE("read()") {
    SUBCASE("reads 4 bytes at a time") {
      stub.setup("ABCDEFG");
      std::string result = callReadSeveralTimes(stream, 7);

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