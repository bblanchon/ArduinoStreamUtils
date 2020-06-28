// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2020
// MIT License

#pragma once

namespace StreamUtils {

inline size_t optimizedRead(Stream &stream, char *buffer, size_t size) {
  // For Stream, use the regular readBytes()
  return stream.readBytes(buffer, size);
}

inline size_t optimizedRead(Client &client, char *buffer, size_t size) {
  // For Client, prefer read() because it's virtual, so it can be optimized by
  // the derived class
  return client.read(reinterpret_cast<uint8_t *>(buffer), size);
}

}  // namespace StreamUtils