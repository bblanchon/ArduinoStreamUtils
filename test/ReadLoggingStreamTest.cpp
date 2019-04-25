// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#include "FailingAllocator.hpp"
#include "Stream.hpp"
#include "StreamSpy.hpp"

#include "StreamUtils/MemoryStream.hpp"
#include "StreamUtils/ReadLoggingStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("ReadLoggingStream") {
  MemoryStream upstream(4);
  StreamSpy upstreamSpy{upstream};
  MemoryStream log(64);
  StreamSpy logSpy{log};
  ReadLoggingStream copier{upstreamSpy, logSpy};

  SUBCASE("available()") {
    upstream.print("ABC");

    size_t n = copier.available();

    CHECK(n == 3);
    CHECK(upstreamSpy.log() == "available() -> 3");
    CHECK(logSpy.log() == "");
  }

  SUBCASE("peek()") {
    upstream.print("ABC");

    int n = copier.peek();

    CHECK(n == 'A');
    CHECK(upstreamSpy.log() == "peek() -> 65");
    CHECK(logSpy.log() == "");
  }

  SUBCASE("read()") {
    upstream.print("ABC");

    int n = copier.read();

    CHECK(n == 'A');
    CHECK(upstreamSpy.log() == "read() -> 65");
    CHECK(logSpy.log() == "write('A') -> 1");
  }

  SUBCASE("readBytes()") {
    upstream.print("ABC");

    char s[4] = {0};
    size_t n = copier.readBytes(s, 4);

    CHECK(n == 3);
    CHECK(upstreamSpy.log() == "readBytes(4) -> 3");
    CHECK(logSpy.log() == "write('ABC', 3) -> 3");
  }

  SUBCASE("write(char)") {
    int n = copier.write('A');

    CHECK(n == 1);
    CHECK(upstreamSpy.log() == "write('A') -> 1");
    CHECK(logSpy.log() == "");
  }

  SUBCASE("write(char*,size_t)") {
    int n = copier.write("ABCDEF", 6);

    CHECK(n == 4);
    CHECK(upstreamSpy.log() == "write('ABCDEF', 6) -> 4");
    CHECK(logSpy.log() == "");
  }

  SUBCASE("flush()") {
    copier.flush();

    CHECK(upstreamSpy.log() == "flush()");
  }
}
