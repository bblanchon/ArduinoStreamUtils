// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2024
// MIT License

#include "StreamUtils/Streams/ChunkDecodingStream.hpp"
#include "StreamUtils/Streams/StringStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("ChunkDecodingStream") {
  StringStream upstream;
  ChunkDecodingStream stream{upstream};

  SUBCASE("empty stream") {
    CHECK(stream.available() == 0);
    CHECK(stream.peek() == -1);
    CHECK(stream.read() == -1);
    CHECK(stream.readString() == "");
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
  }
}
