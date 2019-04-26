// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#include "FailingAllocator.hpp"
#include "Stream.hpp"

#include "StreamUtils/Prints/LoggingPrint.hpp"
#include "StreamUtils/Streams/MemoryStream.hpp"

#include "doctest.h"

using namespace StreamUtils;

TEST_CASE("LoggingPrint") {
  MemoryStream primary(4);
  MemoryStream secondary(64);
  LoggingPrint loggingPrint{primary, secondary};

  SUBCASE("write(char)") {
    int n = loggingPrint.write('A');

    CHECK(n == 1);
    CHECK(primary.readString() == "A");
    CHECK(secondary.readString() == "A");
  }

  SUBCASE("write(char*,size_t)") {
    int n = loggingPrint.write("ABCDEF", 6);

    CHECK(n == 4);
    CHECK(primary.readString() == "ABCD");
    CHECK(secondary.readString() == "ABCD");
  }
}
