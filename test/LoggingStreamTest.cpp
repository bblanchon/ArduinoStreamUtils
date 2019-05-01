// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#include "FailingAllocator.hpp"

#include "StreamUtils/Streams/LoggingStream.hpp"
#include "StreamUtils/Streams/MemoryStream.hpp"
#include "StreamUtils/Streams/SpyingStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("LoggingStream") {
  MemoryStream upstream{4};
  MemoryStream log(64);

  MemoryStream upstreamHistory{64};
  SpyingStream upstreamSpy{upstream, upstreamHistory};

  LoggingStream loggingStream{upstreamSpy, log};

  // upstream -> upstreamSpy -> loggingStream -> log
  //                 |
  //                 v
  //            upsteamHistory

  SUBCASE("available()") {
    upstream.print("ABC");

    size_t n = loggingStream.available();

    CHECK(n == 3);
    CHECK(upstreamHistory.readString() == "available() -> 3");
    CHECK(log.readString() == "");
  }

  SUBCASE("peek()") {
    upstream.print("ABC");

    int n = loggingStream.peek();

    CHECK(n == 'A');
    CHECK(upstreamHistory.readString() == "peek() -> 65");
    CHECK(log.readString() == "");
  }

  SUBCASE("read()") {
    upstream.print("ABC");

    int n = loggingStream.read();

    CHECK(n == 'A');
    CHECK(upstreamHistory.readString() == "read() -> 65");
    CHECK(log.readString() == "A");
  }

  SUBCASE("readBytes()") {
    upstream.print("ABC");

    char s[4] = {0};
    size_t n = loggingStream.readBytes(s, 4);

    CHECK(n == 3);
#if STREAMUTILS_STREAM_READBYTES_IS_VIRTUAL
    CHECK(upstreamHistory.readString() == "readBytes(4) -> 3");
#endif
    CHECK(log.readString() == "ABC");
  }

  SUBCASE("write(char)") {
    int n = loggingStream.write('A');

    CHECK(n == 1);
    CHECK(upstreamHistory.readString() == "write('A') -> 1");
    CHECK(log.readString() == "A");
  }

  SUBCASE("write(char*,size_t)") {
    int n = loggingStream.write("ABCDEF", 6);

    CHECK(n == 4);
    CHECK(upstreamHistory.readString() == "write('ABCDEF', 6) -> 4");
    CHECK(log.readString() == "ABCD");
  }

  SUBCASE("flush()") {
    loggingStream.flush();

    CHECK(upstreamHistory.readString() == "flush()");
    CHECK(log.readString() == "");
  }
}
