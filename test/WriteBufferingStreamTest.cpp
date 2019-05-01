// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#include "FailingAllocator.hpp"

#include "StreamUtils/Streams/MemoryStream.hpp"
#include "StreamUtils/Streams/SpyingStream.hpp"
#include "StreamUtils/Streams/WriteBufferingStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("WriteBufferingStream") {
  MemoryStream upstream(64);

  MemoryStream history(64);
  SpyingStream spy{upstream, history};

  GIVEN("capacity is 4") {
    WriteBufferingStream stream{spy, 4};

    SUBCASE("available()") {
      upstream.print("ABC");

      CHECK(stream.available() == 3);
      CHECK(history.readString() == "available() -> 3");
    }

    SUBCASE("flush() forwards to upstream)") {
      stream.flush();

      CHECK(history.readString() == "flush()");
    }

    SUBCASE("flush() calls write() and flush()") {
      stream.write("ABC", 3);
      stream.flush();

      CHECK(history.readString() ==
            "write('ABC', 3) -> 3"
            "flush()");
    }

    SUBCASE("peek()") {
      upstream.print("ABC");

      CHECK(stream.peek() == 'A');
      CHECK(history.readString() == "peek() -> 65");
    }

    SUBCASE("read()") {
      upstream.print("ABC");

      CHECK(stream.read() == 'A');
      CHECK(history.readString() == "read() -> 65");
    }

    SUBCASE("readBytes()") {
      upstream.print("ABC");

      char s[4] = {0};
      int n = stream.readBytes(s, 3);

      CHECK(n == 3);
      CHECK(s == std::string("ABC"));
#if STREAMUTILS_STREAM_READBYTES_IS_VIRTUAL
      CHECK(history.readString() == "readBytes(3) -> 3");
#endif
    }

    GIVEN("the buffer is empty") {
      SUBCASE("write(uint8_t)") {
        int n = stream.write('A');

        CHECK(n == 1);
        CHECK(history.readString() == "");
      }

      SUBCASE("write(uint8_t) should flush") {
        stream.write('A');
        stream.write('B');
        stream.write('C');
        stream.write('D');
        stream.write('E');

        CHECK(history.readString() == "write('ABCD', 4) -> 4");
      }

      SUBCASE("write(char*,3) goes in buffer") {
        size_t n = stream.write("ABC", 3);

        CHECK(n == 3);
        CHECK(history.readString() == "");
      }

      SUBCASE("write(char*,4) bypasses buffer") {
        size_t n = stream.write("ABCD", 4);

        CHECK(n == 4);
        CHECK(history.readString() == "write('ABCD', 4) -> 4");
      }
      SUBCASE("write(char*,2) bypasses buffer") {
        size_t n = stream.write("ABCD", 4);

        CHECK(n == 4);
        CHECK(history.readString() == "write('ABCD', 4) -> 4");
      }
    }

    GIVEN("one byte in the buffer") {
      stream.write('A');

      SUBCASE("write(char*,3) goes in buffer and flush") {
        size_t n = stream.write("BCD", 3);

        CHECK(n == 3);
        CHECK(history.readString() == "write('ABCD', 4) -> 4");
      }

      SUBCASE("write(char*,7) bypasses") {
        size_t n = stream.write("BCDEFGH", 7);

        CHECK(n == 7);
        CHECK(history.readString() ==
              "write('ABCD', 4) -> 4"
              "write('EFGH', 4) -> 4");
      }
    }
  }

  GIVEN("capacity is 0") {
    WriteBufferingStream stream{spy, 0};

    SUBCASE("write(uint8_t) forwards to upstream") {
      int n = stream.write('X');

      CHECK(n == 1);
      CHECK(history.readString() == "write('X') -> 1");
    }

    SUBCASE("write(char*,1) forwards to upstream") {
      int n = stream.write("A", 1);

      CHECK(n == 1);
      CHECK(history.readString() == "write('A', 1) -> 1");
    }

    SUBCASE("flush() forwards to upstream") {
      stream.flush();

      CHECK(history.readString() == "flush()");
    }
  }

  SUBCASE("Destructor should flush") {
    {
      WriteBufferingStream stream{spy, 10};
      stream.write("ABC", 3);
    }

    CHECK(history.readString() == "write('ABC', 3) -> 3");
  }
}
