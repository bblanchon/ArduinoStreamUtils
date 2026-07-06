// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2026
// MIT License

#include "StreamUtils.hpp"

#if STREAMUTILS_ENABLE_STREAM

#include "SpyingAllocator.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("HammingStream") {
  MemoryStream upstream(64);

  HammingStream<7, 4> stream{upstream};

  SUBCASE("read() decodes") {
    upstream.print("Tq");

    CHECK(stream.read() == 'A');
  }

  SUBCASE("write() encodes") {
    stream.write('A');

    CHECK(upstream.readString() == "Tq");
  }
}
#endif
