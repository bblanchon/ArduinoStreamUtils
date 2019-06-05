// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#include "FailingAllocator.hpp"

#include "StreamUtils/Clients/LoggingClient.hpp"
#include "StreamUtils/Clients/MemoryClient.hpp"
#include "StreamUtils/Clients/SpyingClient.hpp"
#include "StreamUtils/Streams/MemoryStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("LoggingClient") {
  MemoryClient target(4);

  MemoryStream actions(64);
  SpyingClient spy{target, actions};

  MemoryStream log(64);
  LoggingClient loggingClient{spy, log};

  SUBCASE("available()") {
    target.print("ABC");

    size_t n = loggingClient.available();

    CHECK(n == 3);
    CHECK(actions.readString() == "available() -> 3");
    CHECK(log.readString() == "");
  }

  SUBCASE("connect(IPAddress)") {
    int n = loggingClient.connect(IPAddress("1.2.3.4"), 80);

    CHECK(n == 1);
    CHECK(actions.readString() == "connect('1.2.3.4', 80) -> 1");
    CHECK(log.readString() == "");
  }

  SUBCASE("connect(const char*)") {
    int n = loggingClient.connect("1.2.3.4", 80);

    CHECK(n == 1);
    CHECK(actions.readString() == "connect('1.2.3.4', 80) -> 1");
    CHECK(log.readString() == "");
  }

  SUBCASE("connected()") {
    uint8_t n = loggingClient.connected();

    CHECK(n == false);
    CHECK(actions.readString() == "connected() -> 0");
    CHECK(log.readString() == "");
  }

  SUBCASE("stop()") {
    loggingClient.stop();

    CHECK(actions.readString() == "stop()");
    CHECK(log.readString() == "");
  }

  SUBCASE("operator bool()") {
    bool n = loggingClient.operator bool();

    CHECK(n == true);
    CHECK(actions.readString() == "operator bool() -> true");
    CHECK(log.readString() == "");
  }

  SUBCASE("peek()") {
    target.print("ABC");

    int n = loggingClient.peek();

    CHECK(n == 'A');
    CHECK(actions.readString() == "peek() -> 65");
    CHECK(log.readString() == "");
  }

  SUBCASE("read()") {
    target.print("ABC");

    int n = loggingClient.read();

    CHECK(n == 'A');
    CHECK(actions.readString() == "read() -> 65");
    CHECK(log.readString() == "A");
  }

  SUBCASE("read(uint8_t*,size_t)") {
    target.print("ABC");

    uint8_t s[4] = {0};
    size_t n = loggingClient.read(s, 4);

    CHECK(n == 3);
    CHECK(actions.readString() == "read(4) -> 3");
    CHECK(log.readString() == "ABC");
  }

  SUBCASE("readBytes()") {
    target.print("ABC");

    char s[4] = {0};
    size_t n = loggingClient.readBytes(s, 4);

    CHECK(n == 3);
#if STREAMUTILS_STREAM_READBYTES_IS_VIRTUAL
    CHECK(actions.readString() == "readBytes(4) -> 3");
#endif
    CHECK(log.readString() == "ABC");
  }

  SUBCASE("write(char)") {
    int n = loggingClient.write('A');

    CHECK(n == 1);
    CHECK(actions.readString() == "write('A') -> 1");
    CHECK(log.readString() == "A");
  }

  SUBCASE("write(char*,size_t)") {
    int n = loggingClient.write("ABCDEF", 6);

    CHECK(n == 4);
    CHECK(actions.readString() == "write('ABCDEF', 6) -> 4");
    CHECK(log.readString() == "ABCD");
  }

  SUBCASE("flush()") {
    loggingClient.flush();

    CHECK(log.readString() == "");
    CHECK(actions.readString() == "flush()");
  }
}
