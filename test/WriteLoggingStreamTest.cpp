// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#include "FailingAllocator.hpp"

#include "StreamUtils/Streams/MemoryStream.hpp"
#include "StreamUtils/Streams/SpyingStream.hpp"
#include "StreamUtils/Streams/WriteLoggingStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("WriteLoggingStream") {
  MemoryStream upstream(4);

  MemoryStream history(64);
  SpyingStream upstreamSpy{upstream, history};

  MemoryStream log(64);
  WriteLoggingStream loggingStream{upstreamSpy, log};

  SUBCASE("available()") {
    upstream.print("ABC");

    size_t n = loggingStream.available();

    CHECK(n == 3);
    CHECK(history.readString() == "available() -> 3");
    CHECK(log.readString() == "");
  }

  SUBCASE("peek()") {
    upstream.print("ABC");

    int n = loggingStream.peek();

    CHECK(n == 'A');
    CHECK(history.readString() == "peek() -> 65");
    CHECK(log.readString() == "");
  }

  SUBCASE("read()") {
    upstream.print("ABC");

    int n = loggingStream.read();

    CHECK(n == 'A');
    CHECK(history.readString() == "read() -> 65");
    CHECK(log.readString() == "");
  }

  SUBCASE("readBytes()") {
    upstream.print("ABC");

    char s[4] = {0};
    size_t n = loggingStream.readBytes(s, 4);

    CHECK(n == 3);
    CHECK(log.readString() == "");
#if STREAMUTILS_STREAM_READBYTES_IS_VIRTUAL
    CHECK(history.readString() == "readBytes(4) -> 3");
#endif
  }

  SUBCASE("write(char)") {
    int n = loggingStream.write('A');

    CHECK(n == 1);
    CHECK(history.readString() == "write('A') -> 1");
    CHECK(log.readString() == "A");
  }

  SUBCASE("write(char*,size_t)") {
    int n = loggingStream.write("ABCDEF", 6);

    CHECK(n == 4);
    CHECK(history.readString() == "write('ABCDEF', 6) -> 4");
    CHECK(log.readString() == "ABCD");
  }

  SUBCASE("flush()") {
    loggingStream.flush();

    CHECK(history.readString() == "flush()");
    CHECK(log.readString() == "");
  }
}
