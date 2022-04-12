// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2022
// MIT License

#include "FailingAllocator.hpp"

#include "StreamUtils/Prints/StringPrint.hpp"
#include "StreamUtils/Streams/MemoryStream.hpp"
#include "StreamUtils/Streams/SpyingStream.hpp"
#include "StreamUtils/Streams/WriteBufferingStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("WriteBufferingStream") {
  MemoryStream upstream(64);

  StringPrint log;
  SpyingStream spy{upstream, log};

  GIVEN("capacity is 4") {
    WriteBufferingStream stream{spy, 4};

    SUBCASE("available()") {
      upstream.print("ABC");

      CHECK(stream.available() == 3);
      CHECK(log.str() == "available() -> 3");
    }

    SUBCASE("flush() forwards to upstream)") {
      stream.flush();

      CHECK(log.str() == "flush()");
    }

    SUBCASE("flush() calls write() and flush()") {
      stream.write("ABC", 3);
      stream.flush();

      CHECK(log.str() ==
            "write('ABC', 3) -> 3"
            "flush()");
    }

    SUBCASE("peek()") {
      upstream.print("ABC");

      CHECK(stream.peek() == 'A');
      CHECK(log.str() == "peek() -> 65");
    }

    SUBCASE("read()") {
      upstream.print("ABC");

      CHECK(stream.read() == 'A');
      CHECK(log.str() == "read() -> 65");
    }

    SUBCASE("readBytes()") {
      upstream.print("ABC");

      char s[4] = {0};
      size_t n = stream.readBytes(s, 3);

      CHECK(n == 3);
      CHECK(s == std::string("ABC"));
#if STREAMUTILS_STREAM_READBYTES_IS_VIRTUAL
      CHECK(log.str() == "readBytes(3) -> 3");
#endif
    }

    GIVEN("the buffer is empty") {
      SUBCASE("write(uint8_t)") {
        size_t n = stream.write('A');

        CHECK(n == 1);
        CHECK(log.str() == "");
      }

      SUBCASE("write(uint8_t) should flush") {
        stream.write('A');
        stream.write('B');
        stream.write('C');
        stream.write('D');
        stream.write('E');

        CHECK(log.str() == "write('ABCD', 4) -> 4");
      }

      SUBCASE("write(char*,3) goes in buffer") {
        size_t n = stream.write("ABC", 3);

        CHECK(n == 3);
        CHECK(log.str() == "");
      }

      SUBCASE("write(char*,4) bypasses buffer") {
        size_t n = stream.write("ABCD", 4);

        CHECK(n == 4);
        CHECK(log.str() == "write('ABCD', 4) -> 4");
      }
      SUBCASE("write(char*,2) bypasses buffer") {
        size_t n = stream.write("ABCD", 4);

        CHECK(n == 4);
        CHECK(log.str() == "write('ABCD', 4) -> 4");
      }
    }

    GIVEN("one byte in the buffer") {
      stream.write('A');

      SUBCASE("write(char*,3) goes in buffer and flush") {
        size_t n = stream.write("BCD", 3);

        CHECK(n == 3);
        CHECK(log.str() == "write('ABCD', 4) -> 4");
      }

      SUBCASE("write(char*,7) bypasses") {
        size_t n = stream.write("BCDEFGH", 7);

        CHECK(n == 7);
        CHECK(log.str() ==
              "write('ABCD', 4) -> 4"
              "write('EFGH', 4) -> 4");
      }
    }
  }

  GIVEN("capacity is 0") {
    WriteBufferingStream stream{spy, 0};

    SUBCASE("write(uint8_t) forwards to upstream") {
      size_t n = stream.write('X');

      CHECK(n == 1);
      CHECK(log.str() == "write('X') -> 1");
    }

    SUBCASE("write(char*,1) forwards to upstream") {
      size_t n = stream.write("A", 1);

      CHECK(n == 1);
      CHECK(log.str() == "write('A', 1) -> 1");
    }

    SUBCASE("flush() forwards to upstream") {
      stream.flush();

      CHECK(log.str() == "flush()");
    }
  }

  SUBCASE("Destructor should flush") {
    {
      WriteBufferingStream stream{spy, 10};
      stream.write("ABC", 3);
    }

    CHECK(log.str() == "write('ABC', 3) -> 3");
  }
}
