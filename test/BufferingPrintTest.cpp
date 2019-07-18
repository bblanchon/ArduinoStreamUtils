// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#include "FailingAllocator.hpp"

#include "StreamUtils/Prints/BufferingPrint.hpp"
#include "StreamUtils/Prints/SpyingPrint.hpp"
#include "StreamUtils/Streams/MemoryStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("BufferingPrint") {
  MemoryStream target(64);

  MemoryStream actions(64);
  SpyingPrint spy{target, actions};

  GIVEN("capacity is 4") {
    BufferingPrint bufferingPrint{spy, 4};

    SUBCASE("flush() calls write()") {
      bufferingPrint.write("ABC", 3);
      bufferingPrint.flush();

#if STREAMUTILS_PRINT_FLUSH_EXISTS
      CHECK(actions.readString() ==
            "write('ABC', 3) -> 3"
            "flush()");
#else
      CHECK(actions.readString() == "write('ABC', 3) -> 3");
#endif
    }

    GIVEN("the buffer is empty") {
      SUBCASE("write(uint8_t)") {
        int n = bufferingPrint.write('A');

        CHECK(n == 1);
        CHECK(actions.readString() == "");
      }

      SUBCASE("write(uint8_t) should flush") {
        bufferingPrint.write('A');
        bufferingPrint.write('B');
        bufferingPrint.write('C');
        bufferingPrint.write('D');
        bufferingPrint.write('E');

        CHECK(actions.readString() == "write('ABCD', 4) -> 4");
      }

      SUBCASE("write(char*,3) goes in buffer") {
        size_t n = bufferingPrint.write("ABC", 3);

        CHECK(n == 3);
        CHECK(actions.readString() == "");
      }

      SUBCASE("write(char*,4) bypasses buffer") {
        size_t n = bufferingPrint.write("ABCD", 4);

        CHECK(n == 4);
        CHECK(actions.readString() == "write('ABCD', 4) -> 4");
      }
      SUBCASE("write(char*,2) bypasses buffer") {
        size_t n = bufferingPrint.write("ABCD", 4);

        CHECK(n == 4);
        CHECK(actions.readString() == "write('ABCD', 4) -> 4");
      }
    }

    GIVEN("one byte in the buffer") {
      bufferingPrint.write('A');

      SUBCASE("write(char*,3) goes in buffer and flush") {
        size_t n = bufferingPrint.write("BCD", 3);

        CHECK(n == 3);
        CHECK(actions.readString() == "write('ABCD', 4) -> 4");
      }

      SUBCASE("write(char*,7) bypasses") {
        size_t n = bufferingPrint.write("BCDEFGH", 7);

        CHECK(n == 7);
        CHECK(actions.readString() ==
              "write('ABCD', 4) -> 4"
              "write('EFGH', 4) -> 4");
      }
    }
  }

  GIVEN("capacity is 0") {
    BufferingPrint bufferingPrint{spy, 0};

    // SUBCASE("capacity()") {
    //   CHECK(bufferingPrint.capacity() == 0);
    // }

    SUBCASE("write(uint8_t) forwards to target") {
      int n = bufferingPrint.write('X');

      CHECK(n == 1);
      CHECK(actions.readString() == "write('X') -> 1");
    }

    SUBCASE("write(char*,1) forwards to target") {
      int n = bufferingPrint.write("A", 1);

      CHECK(n == 1);
      CHECK(actions.readString() == "write('A', 1) -> 1");
    }

    SUBCASE("flush()") {
      bufferingPrint.flush();

#if STREAMUTILS_PRINT_FLUSH_EXISTS
      CHECK(actions.readString() == "flush()");
#else
      CHECK(actions.readString() == "");
#endif
    }
  }

  SUBCASE("Destructor should flush") {
    {
      BufferingPrint bufferingPrint{spy, 10};
      bufferingPrint.write("ABC", 3);
    }

    CHECK(actions.readString() == "write('ABC', 3) -> 3");
  }
}
