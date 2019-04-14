// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License
//
// This example shows how to log what written to a Stream

#include <StreamUtils.h>

// Everything we write to serial, will be written twice
auto loggedStream = logOutput(Serial, Serial);

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial)
    continue;
}

void loop() {
  // Even if it looks like the bytes are extracted one by one, they are actual
  // read by chunks in 64 bytes and placed in a buffer.
  while (Serial.available()) {
    loggedStream.write(Serial.read());
  }
}