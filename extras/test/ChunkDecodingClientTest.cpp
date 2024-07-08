// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2024
// MIT License

#include "StreamUtils/Clients/ChunkDecodingClient.hpp"
#include "StreamUtils/Clients/MemoryClient.hpp"
#include "StreamUtils/Clients/SpyingClient.hpp"
#include "StreamUtils/Prints/StringPrint.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("ChunkDecodingClient") {
  MemoryClient upstream(128);
  StringPrint log;
  SpyingClient spy{upstream, log};
  ChunkDecodingClient client{spy};

  // Most of the tests are in ChunkDecodingStreamTest.cpp
  // This file only tests the client-specific methods

  SUBCASE("read() merge chunks") {
    uint8_t buffer[32];

    upstream.print(
        "4\r\n"
        "XXXX\r\n"
        "4\r\n"
        "YYYY\r\n"
        "0\r\n"
        "\r\n");
    REQUIRE(client.read(buffer, 32) == 8);
    REQUIRE(buffer[0] == 'X');
    REQUIRE(buffer[4] == 'Y');
  }

  SUBCASE("read() waits timeout") {
    uint8_t buffer[32];

    upstream.print(
        "4\r\n"
        "XXXX\r\n");
    REQUIRE(client.read(buffer, 32) == 4);
    REQUIRE(buffer[0] == 'X');

    REQUIRE(log.str() ==
            "read(1) -> 1"  // 1
            "read(1) -> 1"  // \r
            "read(1) -> 1"  // \n
            "read(4) -> 4"  // XXXX
            "read(1) -> 1"  // \r
            "read(1) -> 1"  // \n
            "read(1) -> 0 [timeout]");
  }

  SUBCASE("read() doen't wait if final chunk received") {
    uint8_t buffer[32];

    upstream.print(
        "4\r\n"
        "XXXX\r\n"
        "0\r\n"
        "\r\n");
    REQUIRE(client.read(buffer, 32) == 4);
    REQUIRE(buffer[0] == 'X');
    REQUIRE(client.ended() == true);

    REQUIRE(log.str() ==
            "read(1) -> 1"  // 1
            "read(1) -> 1"  // \r
            "read(1) -> 1"  // \n
            "read(4) -> 4"  // XXXX
            "read(1) -> 1"  // \r
            "read(1) -> 1"  // \n
            "read(1) -> 1"  // 0
            "read(1) -> 1"  // \r
            "read(1) -> 1"  // \n
            "read(1) -> 1"  // \r
            "read(1) -> 1"  // \n
    );
  }
}
