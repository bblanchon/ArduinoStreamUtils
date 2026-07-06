// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2026
// MIT License

#pragma once

#include "./Configuration.hpp"

#if STREAMUTILS_ENABLE_STREAM
#include <Stream.h>
#endif
#if STREAMUTILS_ENABLE_CLIENT
#include <Client.h>
#endif

namespace StreamUtils {

#if STREAMUTILS_ENABLE_STREAM
inline size_t readOrReadBytes(Stream &stream, char *buffer, size_t size) {
  return stream.readBytes(buffer, size);
}
#endif

#if STREAMUTILS_ENABLE_CLIENT
inline size_t readOrReadBytes(Client &client, char *buffer, size_t size) {
  return client.read(reinterpret_cast<uint8_t *>(buffer), size);
}
#endif

}  // namespace StreamUtils
