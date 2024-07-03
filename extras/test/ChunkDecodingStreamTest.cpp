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

  WHEN("upstream is empty") {
    THEN("available() should return 0") {
      CHECK(stream.available() == 0);
    }
  }
}
