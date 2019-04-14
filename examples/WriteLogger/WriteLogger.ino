// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License
//
// This example shows how to log what written to a Stream

#include <StreamUtils.h>

// Create a new stream that will forward all calls to Serial, and log to Serial.
// Everything will be written twice to the Serial!
WriteLoggingStream loggingStream(Serial, Serial);

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial)
    continue;
}

void loop() {
  // Read from the serial port and write back.
  // Because loggingStream logs each write operation, everything is written
  // twice.
  while (loggingStream.available()) {
    loggingStream.read();
  }
}