// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2020
// MIT License

#include "FailingAllocator.hpp"

#include "StreamUtils/Clients/MemoryClient.hpp"
#include "StreamUtils/Clients/ReadBufferingClient.hpp"
#include "StreamUtils/Clients/SpyingClient.hpp"
#include "StreamUtils/Prints/StringPrint.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("ReadBufferingClient") {
  MemoryClient target(64);
  StringPrint log;
  SpyingClient spy{target, log};

  SUBCASE("capacity = 4") {
    ReadBufferingClient bufferedClient{spy, 4};
    Client& client = bufferedClient;

    SUBCASE("available()") {
      target.print("ABCDEFGH");

      SUBCASE("empty input") {
        target.flush();
        CHECK(client.available() == 0);
        CHECK(log.str() == "available() -> 0");
      }

      SUBCASE("read empty input") {
        target.flush();

        client.read();

        CHECK(client.available() == 0);
        CHECK(log.str() ==
              "read(4) -> 0"
              "available() -> 0");
      }

      SUBCASE("same a target") {
        CHECK(client.available() == 8);
        CHECK(log.str() == "available() -> 8");
      }

      SUBCASE("target + in buffer") {
        client.read();

        CHECK(client.available() == 7);
        CHECK(log.str() ==
              "read(4) -> 4"
              "available() -> 4");
      }
    }

    SUBCASE("peek()") {
      SUBCASE("returns -1 when empty") {
        target.flush();

        int result = client.peek();

        CHECK(result == -1);
        CHECK(log.str() == "peek() -> -1");
      }

      SUBCASE("doesn't call readBytes() when buffer is empty") {
        target.print("A");

        int result = client.peek();

        CHECK(result == 'A');
        CHECK(log.str() == "peek() -> 65");
      }

      SUBCASE("doesn't call peek() when buffer is full") {
        target.print("AB");

        client.read();
        int result = client.peek();

        CHECK(result == 'B');
        CHECK(log.str() == "read(4) -> 2");
      }
    }

    SUBCASE("read()") {
      SUBCASE("reads 4 bytes at a time") {
        target.print("ABCDEFG");
        std::string result;

        for (int i = 0; i < 7; i++) {
          result += (char)client.read();
        }

        CHECK(result == "ABCDEFG");
        CHECK(log.str() ==
              "read(4) -> 4"
              "read(4) -> 3");
      }

      SUBCASE("returns -1 when empty") {
        target.flush();

        int result = client.read();

        CHECK(result == -1);
        CHECK(log.str() == "read(4) -> 0");
      }
    }

    SUBCASE("read()") {
      SUBCASE("empty input") {
        target.flush();

        char c;
        size_t result = client.readBytes(&c, 1);

        CHECK(result == 0);
        CHECK(log.str() == "read(4) -> 0");
      }

      SUBCASE("reads 4 bytes when requested one") {
        target.print("ABCDEFG");

        char c;
        size_t result = client.readBytes(&c, 1);

        CHECK(c == 'A');
        CHECK(result == 1);
        CHECK(log.str() == "read(4) -> 4");
      }

      SUBCASE("copy one byte from buffer") {
        target.print("ABCDEFGH");
        client.read();  // load buffer

        char c;
        size_t result = client.readBytes(&c, 1);

        CHECK(c == 'B');
        CHECK(result == 1);
        CHECK(log.str() == "read(4) -> 4");
      }

      SUBCASE("copy content from buffer then bypass buffer") {
        target.print("ABCDEFGH");
        client.read();  // load buffer

        char c[8] = {0};
        size_t result = client.readBytes(c, 7);

        CHECK(c == std::string("BCDEFGH"));
        CHECK(result == 7);
        CHECK(log.str() ==
              "read(4) -> 4"
              "read(4) -> 4");
      }

      SUBCASE("copy content from buffer twice") {
        target.print("ABCDEFGH");
        client.read();  // load buffer

        char c[8] = {0};
        size_t result = client.readBytes(c, 4);

        CHECK(c == std::string("BCDE"));
        CHECK(result == 4);
        CHECK(log.str() ==
              "read(4) -> 4"
              "read(4) -> 4");
      }

      SUBCASE("read past the end") {
        target.print("A");

        char c;
        client.readBytes(&c, 1);
        size_t result = client.readBytes(&c, 1);

        CHECK(result == 0);
        CHECK(log.str() ==
              "read(4) -> 1"
              "read(4) -> 0");
      }
    }

    SUBCASE("flush()") {
      client.flush();
      CHECK(log.str() == "flush()");
    }

    SUBCASE("copy constructor") {
      target.print("ABCDEFGH");
      bufferedClient.read();

      auto dup = bufferedClient;

      int result = dup.read();

      CHECK(result == 'B');
      CHECK(log.str() == "read(4) -> 4");
    }
  }

  SUBCASE("No memory") {
    BasicReadBufferingClient<FailingAllocator> client(spy, 4);

    SUBCASE("available()") {
      target.print("ABC");

      CHECK(client.available() == 3);
    }

    // SUBCASE("capacity()") {
    //   CHECK(client.capacity() == 0);
    // }

    SUBCASE("peek()") {
      target.print("ABC");

      int c = client.peek();

      CHECK(c == 'A');
      CHECK(log.str() == "peek() -> 65");
    }

    SUBCASE("read()") {
      target.print("ABC");

      int c = client.read();

      CHECK(c == 'A');
      CHECK(log.str() == "read() -> 65");
    }

    SUBCASE("readBytes()") {
      target.print("ABC");

      char s[4] = {0};
      int n = client.readBytes(s, 3);

      CHECK(n == 3);
      CHECK(s == std::string("ABC"));
#if STREAMUTILS_STREAM_READBYTES_IS_VIRTUAL
      CHECK(log.str() == "read(3) -> 3");
#endif
    }
  }

  SUBCASE("Real example") {
    ReadBufferingClient bufferedClient{spy, 64};
    Client& client = bufferedClient;

    target.print("{\"helloWorld\":\"Hello World\"}");

    char c[] = "ABCDEFGH";
    CHECK(client.readBytes(&c[0], 1) == 1);
    CHECK(client.readBytes(&c[1], 1) == 1);
    CHECK(client.readBytes(&c[2], 1) == 1);
    CHECK(client.readBytes(&c[3], 1) == 1);

    CHECK(c == std::string("{\"heEFGH"));
    CHECK(log.str() == "read(64) -> 28");
  }
}
