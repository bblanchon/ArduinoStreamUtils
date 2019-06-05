// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#ifndef STREAMUTILS_PRINT_FLUSH_EXISTS
#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_SAMD)
#define STREAMUTILS_PRINT_FLUSH_EXISTS 1
#else
#define STREAMUTILS_PRINT_FLUSH_EXISTS 0
#endif
#endif

#ifndef STREAMUTILS_STREAM_READBYTES_IS_VIRTUAL
#if defined(ARDUINO_ARCH_ESP8266)
#define STREAMUTILS_STREAM_READBYTES_IS_VIRTUAL 1
#else
#define STREAMUTILS_STREAM_READBYTES_IS_VIRTUAL 0
#endif
#endif