// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2024
// MIT License

#include "StreamUtils/Prints/StringPrint.hpp"
#include "StreamUtils/Streams/ChunkDecodingStream.hpp"
#include "StreamUtils/Streams/SpyingStream.hpp"
#include "StreamUtils/Streams/StringStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("ChunkDecodingStream") {
  StringStream upstream;
  StringPrint log;
  SpyingStream spy{upstream, log};
  ChunkDecodingStream stream{spy};

  SUBCASE("empty stream") {
    CHECK(stream.available() == 0);
    CHECK(stream.peek() == -1);
    CHECK(stream.read() == -1);
    CHECK(stream.readString() == "");
    CHECK(stream.error() == false);
  }

  SUBCASE("sizes") {
    char buffer[32];

    upstream.print(
        "1\r\n"
        "a\r\n");
    REQUIRE(stream.available() == 1);
    REQUIRE(stream.peek() == 'a');
    REQUIRE(stream.readBytes(buffer, sizeof(buffer)) == 1);
    REQUIRE(buffer[0] == 'a');

    upstream.print(
        "2\r\n"
        "ab\r\n");
    REQUIRE(stream.available() == 2);
    REQUIRE(stream.peek() == 'a');
    REQUIRE(stream.readBytes(buffer, sizeof(buffer)) == 2);
    REQUIRE(buffer[0] == 'a');
    REQUIRE(buffer[1] == 'b');

    upstream.print(
        "3\r\n"
        "abc\r\n");
    REQUIRE(stream.available() == 3);
    REQUIRE(stream.peek() == 'a');
    REQUIRE(stream.readBytes(buffer, sizeof(buffer)) == 3);
    REQUIRE(buffer[0] == 'a');
    REQUIRE(buffer[2] == 'c');

    upstream.print(
        "9\r\n"
        "abcdefghi\r\n");
    REQUIRE(stream.readBytes(buffer, sizeof(buffer)) == 9);
    REQUIRE(buffer[0] == 'a');
    REQUIRE(buffer[8] == 'i');

    upstream.print(
        "a\r\n"
        "abcdefghij\r\n");
    REQUIRE(stream.readBytes(buffer, sizeof(buffer)) == 10);
    REQUIRE(buffer[0] == 'a');
    REQUIRE(buffer[9] == 'j');

    upstream.print(
        "A\r\n"
        "abcdefghij\r\n");
    REQUIRE(stream.readBytes(buffer, sizeof(buffer)) == 10);
    REQUIRE(buffer[0] == 'a');
    REQUIRE(buffer[9] == 'j');

    upstream.print(
        "f\r\n"
        "abcdefghijklmno\r\n");
    REQUIRE(stream.readBytes(buffer, sizeof(buffer)) == 15);
    REQUIRE(buffer[0] == 'a');
    REQUIRE(buffer[14] == 'o');

    upstream.print(
        "F\r\n"
        "abcdefghijklmno\r\n");
    REQUIRE(stream.readBytes(buffer, sizeof(buffer)) == 15);
    REQUIRE(buffer[0] == 'a');
    REQUIRE(buffer[14] == 'o');

    upstream.print(
        "10\r\n"
        "abcdefghijklmnop\r\n");
    REQUIRE(stream.readBytes(buffer, sizeof(buffer)) == 16);
    REQUIRE(buffer[0] == 'a');
    REQUIRE(buffer[15] == 'p');

    // final chunk
    upstream.print(
        "0\r\n"
        "\r\n");
    REQUIRE(stream.available() == 0);
    REQUIRE(stream.peek() == -1);
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == false);
  }

  SUBCASE("extensions") {
    // no space
    upstream.print(
        "1;foo=bar\r\n"
        "X\r\n");
    REQUIRE(stream.available() == 1);
    REQUIRE(stream.read() == 'X');

    // space before semicolon
    upstream.print(
        "1 ;foo=bar\r\n"
        "X\r\n");
    REQUIRE(stream.available() == 1);
    REQUIRE(stream.read() == 'X');

    // tab before semicolon
    upstream.print(
        "1\t;foo=bar\r\n"
        "X\r\n");
    REQUIRE(stream.available() == 1);
    REQUIRE(stream.read() == 'X');

    // final chunk
    upstream.print(
        "0;foo=bar\r\n"
        "\r\n");
    REQUIRE(stream.available() == 0);
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == false);
  }

  SUBCASE("trailers") {
    upstream.print(
        "0\r\n"
        "foo: bar\r\n"
        "baz: qux\r\n"
        "\r\n");
    REQUIRE(stream.available() == 0);
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == false);
  }

  SUBCASE("restart after trailers") {
    upstream.print(
        "0\r\n"
        "foo: bar\r\n"
        "baz: qux\r\n"
        "\r\n"
        "1\r\n"
        "X\r\n");
    REQUIRE(stream.available() == 0);
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == true);
  }

  SUBCASE("size starts with non hexadecimal digit") {
    upstream.print(
        "G\r\n"
        "XXXXXX\r\n");
    REQUIRE(stream.available() == 0);
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == true);
  }

  SUBCASE("size ends with non hexadecimal digit") {
    upstream.print(
        "1G\r\n"
        "XXXXXX\r\n");
    REQUIRE(stream.available() == 0);
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == true);
  }

  SUBCASE("CR missing after size") {
    upstream.print(
        "1\n"
        "X\r\n");
    REQUIRE(stream.available() == 0);
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == true);
  }

  SUBCASE("LF missing after size") {
    upstream.print(
        "1\r"
        "X\r\n");
    REQUIRE(stream.available() == 0);
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == true);
  }

  SUBCASE("chunk shorter than announced") {
    upstream.print(
        "2\r\n"
        "X\r\n");
    REQUIRE(stream.available() == 2);
    REQUIRE(stream.read() == 'X');
    REQUIRE(stream.read() == '\r');
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == true);
  }

  SUBCASE("chunk longer than announced") {
    upstream.print(
        "1\r\n"
        "XX\r\n");
    REQUIRE(stream.available() == 1);
    REQUIRE(stream.read() == 'X');
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == true);
  }

  SUBCASE("CR missing after chunk") {
    upstream.print(
        "1\r\n"
        "X\n"
        "0\r\n"
        "\r\n");
    REQUIRE(stream.available() == 1);
    REQUIRE(stream.read() == 'X');
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == true);
  }

  SUBCASE("LF missing after chunk") {
    upstream.print(
        "1\r\n"
        "X\r"
        "0\r\n"
        "\r\n");
    REQUIRE(stream.available() == 1);
    REQUIRE(stream.read() == 'X');
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == true);
  }

  SUBCASE("CR missing after trailer") {
    upstream.print(
        "0\r\n"
        "foo: bar\n"
        "\r\n");
    REQUIRE(stream.available() == 0);
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == true);
  }

  SUBCASE("LF missing after trailer") {
    upstream.print(
        "0\r\n"
        "foo: bar\r"
        "\r\n");
    REQUIRE(stream.available() == 0);
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == true);
  }

  SUBCASE("final LF missing") {
    upstream.print(
        "0\r\n"
        "\r"
        "1\r\n");
    REQUIRE(stream.available() == 0);
    REQUIRE(stream.read() == -1);
    REQUIRE(stream.error() == true);
  }

  SUBCASE("size overflow") {
    upstream.print(
        "10000000000000001\r\n"
        "X\r\n");
    CHECK(stream.available() == 0);
    CHECK(stream.read() == -1);
    CHECK(stream.error() == true);
  }

  SUBCASE("readBytes() waits timeout") {
    char buffer[32];
    upstream.print(
        "1\r\n"
        "X\r\n");
    REQUIRE(stream.readBytes(buffer, 2) == 1);
    REQUIRE(buffer[0] == 'X');
    CHECK(log.str() ==
          "read() -> 49"
          "read() -> 13"
          "read() -> 10"
          "read() -> 88"
          "read() -> 13"
          "read() -> 10"
          "read() -> -1");
  }
}
