// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#include "FailingAllocator.hpp"
#include "Stream.hpp"
#include "StreamSpy.hpp"

#include "StreamUtils/Streams/MemoryStream.hpp"
#include "StreamUtils/Streams/WriteLoggingStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("WriteLoggingStream") {
  MemoryStream upstream(4);

  MemoryStream history(64);
  StreamSpy upstreamSpy{upstream, history};

  MemoryStream log(64);
  WriteLoggingStream copier{upstreamSpy, log};

  SUBCASE("available()") {
    upstream.print("ABC");

    size_t n = copier.available();

    CHECK(n == 3);
    CHECK(history.readString() == "available() -> 3");
    CHECK(log.readString() == "");
  }

  SUBCASE("peek()") {
    upstream.print("ABC");

    int n = copier.peek();

    CHECK(n == 'A');
    CHECK(history.readString() == "peek() -> 65");
    CHECK(log.readString() == "");
  }

  SUBCASE("read()") {
    upstream.print("ABC");

    int n = copier.read();

    CHECK(n == 'A');
    CHECK(history.readString() == "read() -> 65");
    CHECK(log.readString() == "");
  }

  SUBCASE("readBytes()") {
    upstream.print("ABC");

    char s[4] = {0};
    size_t n = copier.readBytes(s, 4);

    CHECK(n == 3);
    CHECK(history.readString() == "readBytes(4) -> 3");
    CHECK(log.readString() == "");
  }

  SUBCASE("write(char)") {
    int n = copier.write('A');

    CHECK(n == 1);
    CHECK(history.readString() == "write('A') -> 1");
    CHECK(log.readString() == "A");
  }

  SUBCASE("write(char*,size_t)") {
    int n = copier.write("ABCDEF", 6);

    CHECK(n == 4);
    CHECK(history.readString() == "write('ABCDEF', 6) -> 4");
    CHECK(log.readString() == "ABCD");
  }

  SUBCASE("flush()") {
    copier.flush();

    CHECK(history.readString() == "flush()");
    CHECK(log.readString() == "");
  }
}
