// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2024
// MIT License

#include "StreamUtils/Streams/ChunkDecodingStream.hpp"
#include "StreamUtils/Streams/StringStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("ChunkDecodingStream") {
  StringStream upstream;
  ChunkDecodingStream stream{upstream};

  SUBCASE("empty stream") {
    CHECK(stream.available() == 0);
    CHECK(stream.peek() == -1);
    CHECK(stream.read() == -1);
    CHECK(stream.readString() == "");
  }

  SUBCASE("one chunk, no extension") {
    upstream.print("3\r\nABC");
    CHECK(stream.available() == 3);
    CHECK(stream.peek() == 'A');
    CHECK(stream.available() == 3);
    CHECK(stream.read() == 'A');
    CHECK(stream.available() == 2);
    CHECK(stream.readString() == "BC");
    CHECK(stream.available() == 0);
  }
}
