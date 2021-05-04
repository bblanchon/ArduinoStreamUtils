// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2021
// MIT License

#pragma once

#include <Client.h>
#include <Print.h>
#include <Stream.h>
#include <WString.h>

#include <time.h>

inline unsigned long millis() {
  return static_cast<unsigned long>(time(NULL));
}

inline void yield() {}