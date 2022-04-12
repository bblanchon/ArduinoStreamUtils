// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2022
// MIT License

#include "FailingAllocator.hpp"

#include "StreamUtils/Prints/StringPrint.hpp"
#include "StreamUtils/Streams/MemoryStream.hpp"
#include "StreamUtils/Streams/ReadLoggingStream.hpp"
#include "StreamUtils/Streams/SpyingStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("ReadLoggingStream") {
  MemoryStream upstream(4);

  StringPrint log;
  SpyingStream upstreamSpy{upstream, log};

  StringPrint output;
  ReadLoggingStream loggingStream{upstreamSpy, output};

  SUBCASE("available()") {
    upstream.print("ABC");

    size_t n = loggingStream.available();

    CHECK(n == 3);
    CHECK(log.str() == "available() -> 3");
    CHECK(output.str() == "");
  }

  SUBCASE("peek()") {
    upstream.print("ABC");

    int n = loggingStream.peek();

    CHECK(n == 'A');
    CHECK(log.str() == "peek() -> 65");
    CHECK(output.str() == "");
  }

  SUBCASE("read()") {
    upstream.print("ABC");

    int n = loggingStream.read();

    CHECK(n == 'A');
    CHECK(log.str() == "read() -> 65");
    CHECK(output.str() == "A");
  }

  SUBCASE("readBytes()") {
    upstream.print("ABC");

    char s[4] = {0};
    size_t n = loggingStream.readBytes(s, 4);

    CHECK(n == 3);
    CHECK(output.str() == "ABC");
#if STREAMUTILS_STREAM_READBYTES_IS_VIRTUAL
    CHECK(log.str() == "readBytes(4) -> 3 [timeout]");
#endif
  }

  SUBCASE("write(char)") {
    size_t n = loggingStream.write('A');

    CHECK(n == 1);
    CHECK(log.str() == "write('A') -> 1");
    CHECK(output.str() == "");
  }

  SUBCASE("write(char*,size_t)") {
    size_t n = loggingStream.write("ABCDEF", 6);

    CHECK(n == 4);
    CHECK(log.str() == "write('ABCDEF', 6) -> 4");
    CHECK(output.str() == "");
  }

  SUBCASE("flush()") {
    loggingStream.flush();

    CHECK(log.str() == "flush()");
  }
}
