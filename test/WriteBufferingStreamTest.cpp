// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#include "FailingAllocator.hpp"
#include "Stream.hpp"
#include "StreamSpy.hpp"

#include "StreamUtils/MemoryStream.hpp"
#include "StreamUtils/WriteBufferingStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("WriteBufferingStream") {
  MemoryStream upstream(64);
  StreamSpy spy{upstream};

  GIVEN("capacity is 4") {
    WriteBufferingStream stream{spy, 4};

    SUBCASE("available()") {
      upstream.print("ABC");

      CHECK(stream.available() == 3);
      CHECK(spy.log() == "available() -> 3");
    }

    // SUBCASE("capacity()") {
    //   CHECK(stream.capacity() == 4);
    // }

    SUBCASE("flush() forwards to upstream)") {
      stream.flush();

      CHECK(spy.log() == "flush()");
    }

    SUBCASE("flush() calls write() and flush()") {
      stream.write("ABC", 3);
      stream.flush();

      CHECK(spy.log() ==
            "write('ABC', 3) -> 3"
            "flush()");
    }

    SUBCASE("peek()") {
      upstream.print("ABC");

      CHECK(stream.peek() == 'A');
      CHECK(spy.log() == "peek() -> 65");
    }

    SUBCASE("read()") {
      upstream.print("ABC");

      CHECK(stream.read() == 'A');
      CHECK(spy.log() == "read() -> 65");
    }

    SUBCASE("readBytes()") {
      upstream.print("ABC");

      char s[4] = {0};
      int n = stream.readBytes(s, 3);

      CHECK(n == 3);
      CHECK(s == std::string("ABC"));
      CHECK(spy.log() == "readBytes(3) -> 3");
    }

    GIVEN("the buffer is empty") {
      SUBCASE("write(uint8_t)") {
        int n = stream.write('A');

        CHECK(n == 1);
        CHECK(spy.log() == "");
      }

      SUBCASE("write(uint8_t) should flush") {
        stream.write('A');
        stream.write('B');
        stream.write('C');
        stream.write('D');
        stream.write('E');

        CHECK(spy.log() == "write('ABCD', 4) -> 4");
      }

      SUBCASE("write(char*,3) goes in buffer") {
        size_t n = stream.write("ABC", 3);

        CHECK(n == 3);
        CHECK(spy.log() == "");
      }

      SUBCASE("write(char*,4) bypasses buffer") {
        size_t n = stream.write("ABCD", 4);

        CHECK(n == 4);
        CHECK(spy.log() == "write('ABCD', 4) -> 4");
      }
      SUBCASE("write(char*,2) bypasses buffer") {
        size_t n = stream.write("ABCD", 4);

        CHECK(n == 4);
        CHECK(spy.log() == "write('ABCD', 4) -> 4");
      }
    }

    GIVEN("one byte in the buffer") {
      stream.write('A');

      SUBCASE("write(char*,3) goes in buffer and flush") {
        size_t n = stream.write("BCD", 3);

        CHECK(n == 3);
        CHECK(spy.log() == "write('ABCD', 4) -> 4");
      }

      SUBCASE("write(char*,7) bypasses") {
        size_t n = stream.write("BCDEFGH", 7);

        CHECK(n == 7);
        CHECK(spy.log() ==
              "write('ABCD', 4) -> 4"
              "write('EFGH', 4) -> 4");
      }
    }
  }

  GIVEN("capacity is 0") {
    WriteBufferingStream stream{spy, 0};

    // SUBCASE("capacity()") {
    //   CHECK(stream.capacity() == 0);
    // }

    SUBCASE("write(uint8_t) forwards to upstream") {
      int n = stream.write('X');

      CHECK(n == 1);
      CHECK(spy.log() == "write('X') -> 1");
    }

    SUBCASE("write(char*,1) forwards to upstream") {
      int n = stream.write("A", 1);

      CHECK(n == 1);
      CHECK(spy.log() == "write('A', 1) -> 1");
    }

    SUBCASE("flush() forwards to upstream") {
      stream.flush();

      CHECK(spy.log() == "flush()");
    }
  }

  SUBCASE("Destructor should flush") {
    {
      WriteBufferingStream stream{spy, 10};
      stream.write("ABC", 3);
    }

    CHECK(spy.log() == "write('ABC', 3) -> 3");
  }
}
