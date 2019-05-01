// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#include "FailingAllocator.hpp"

#include "StreamUtils/Clients/MemoryClient.hpp"
#include "StreamUtils/Clients/SpyingClient.hpp"
#include "StreamUtils/Clients/WriteBufferingClient.hpp"
#include "StreamUtils/Streams/MemoryStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("WriteBufferingClient") {
  MemoryClient target(64);

  MemoryStream actions(64);
  SpyingClient spy{target, actions};

  GIVEN("capacity is 4") {
    WriteBufferingClient bufferingClient{spy, 4};

    SUBCASE("available()") {
      target.print("ABC");

      CHECK(bufferingClient.available() == 3);
      CHECK(actions.readString() == "available() -> 3");
    }

    SUBCASE("connect(IPAddress)") {
      int n = bufferingClient.connect(IPAddress("1.2.3.4"), 80);

      CHECK(n == 1);
      CHECK(actions.readString() == "connect('1.2.3.4', 80) -> 1");
    }

    SUBCASE("connect(const char*)") {
      int n = bufferingClient.connect("1.2.3.4", 80);

      CHECK(n == 1);
      CHECK(actions.readString() == "connect('1.2.3.4', 80) -> 1");
    }

    SUBCASE("connected()") {
      uint8_t n = bufferingClient.connected();

      CHECK(n == false);
      CHECK(actions.readString() == "connected() -> 0");
    }

#if STREAMUTILS_CLIENT_STOP_TAKES_TIMEOUT
    SUBCASE("stop(unsigned)") {
      bufferingClient.write("ABC", 3);
      bool result = bufferingClient.stop(10);

      CHECK(result == true);
      CHECK(actions.readString() ==
            "write('ABC', 3) -> 3"
            "stop(10) -> true");
    }
#else
    SUBCASE("stop()") {
      bufferingClient.write("ABC", 3);
      bufferingClient.stop();

      CHECK(actions.readString() ==
            "write('ABC', 3) -> 3"
            "stop()");
    }
#endif

    SUBCASE("operator bool()") {
      bool n = bufferingClient.operator bool();

      CHECK(n == true);
      CHECK(actions.readString() == "operator bool() -> true");
    }

#if STREAMUTILS_CLIENT_FLUSH_TAKES_TIMEOUT
    SUBCASE("flush(unsigned) forwards to target)") {
      bufferingClient.flush(123);

      CHECK(actions.readString() == "flush(123) -> true");
    }
#endif

    SUBCASE("flush() forwards to target)") {
      bufferingClient.flush();

#if STREAMUTILS_CLIENT_FLUSH_TAKES_TIMEOUT
      CHECK(actions.readString() == "flush(0) -> true");
#else
      CHECK(actions.readString() == "flush()");
#endif
    }

    SUBCASE("flush() calls write() and flush()") {
      bufferingClient.write("ABC", 3);
      bufferingClient.flush();

      CHECK(actions.readString() ==
            "write('ABC', 3) -> 3"
#if STREAMUTILS_CLIENT_FLUSH_TAKES_TIMEOUT
            "flush(0) -> true");
#else
            "flush()");
#endif
    }

    SUBCASE("peek()") {
      target.print("ABC");

      CHECK(bufferingClient.peek() == 'A');
      CHECK(actions.readString() == "peek() -> 65");
    }

    SUBCASE("read()") {
      target.print("ABC");

      CHECK(bufferingClient.read() == 'A');
      CHECK(actions.readString() == "read() -> 65");
    }

    SUBCASE("readBytes()") {
      target.print("ABC");

      char s[4] = {0};
      int n = bufferingClient.readBytes(s, 3);

      CHECK(n == 3);
      CHECK(s == std::string("ABC"));
#if STREAMUTILS_STREAM_READBYTES_IS_VIRTUAL
      CHECK(actions.readString() == "readBytes(3) -> 3");
#endif
    }

    GIVEN("the buffer is empty") {
      SUBCASE("write(uint8_t)") {
        int n = bufferingClient.write('A');

        CHECK(n == 1);
        CHECK(actions.readString() == "");
      }

      SUBCASE("write(uint8_t) should flush") {
        bufferingClient.write('A');
        bufferingClient.write('B');
        bufferingClient.write('C');
        bufferingClient.write('D');
        bufferingClient.write('E');

        CHECK(actions.readString() == "write('ABCD', 4) -> 4");
      }

      SUBCASE("write(char*,3) goes in buffer") {
        size_t n = bufferingClient.write("ABC", 3);

        CHECK(n == 3);
        CHECK(actions.readString() == "");
      }

      SUBCASE("write(char*,4) bypasses buffer") {
        size_t n = bufferingClient.write("ABCD", 4);

        CHECK(n == 4);
        CHECK(actions.readString() == "write('ABCD', 4) -> 4");
      }
      SUBCASE("write(char*,2) bypasses buffer") {
        size_t n = bufferingClient.write("ABCD", 4);

        CHECK(n == 4);
        CHECK(actions.readString() == "write('ABCD', 4) -> 4");
      }
    }

    GIVEN("one byte in the buffer") {
      bufferingClient.write('A');

      SUBCASE("write(char*,3) goes in buffer and flush") {
        size_t n = bufferingClient.write("BCD", 3);

        CHECK(n == 3);
        CHECK(actions.readString() == "write('ABCD', 4) -> 4");
      }

      SUBCASE("write(char*,7) bypasses") {
        size_t n = bufferingClient.write("BCDEFGH", 7);

        CHECK(n == 7);
        CHECK(actions.readString() ==
              "write('ABCD', 4) -> 4"
              "write('EFGH', 4) -> 4");
      }
    }
  }

  GIVEN("capacity is 0") {
    WriteBufferingClient bufferingClient{spy, 0};

    SUBCASE("write(uint8_t) forwards to target") {
      int n = bufferingClient.write('X');

      CHECK(n == 1);
      CHECK(actions.readString() == "write('X') -> 1");
    }

    SUBCASE("write(char*,1) forwards to target") {
      int n = bufferingClient.write("A", 1);

      CHECK(n == 1);
      CHECK(actions.readString() == "write('A', 1) -> 1");
    }

    SUBCASE("flush() forwards to target") {
      bufferingClient.flush();

#if STREAMUTILS_CLIENT_FLUSH_TAKES_TIMEOUT
      CHECK(actions.readString() == "flush(0) -> true");
#else
      CHECK(actions.readString() == "flush()");
#endif
    }
  }

  SUBCASE("Destructor should flush") {
    {
      WriteBufferingClient bufferingClient{spy, 10};
      bufferingClient.write("ABC", 3);
    }

    CHECK(actions.readString() == "write('ABC', 3) -> 3");
  }
}
