// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License
//
// This example shows how to log what read from a Stream

#include <StreamUtils.h>

// Create a new stream that will forward all calls to Serial, and log to Serial.
// It will write back everything it reads, just like "echo"
ReadLoggingStream loggingStream(Serial, Serial);

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial)
    continue;
}

void loop() {
  // Read the serial port.
  // Because loggingStream write everything it read, the program will show what
  // you sent.
  while (Serial.available()) {
    loggingStream.write(Serial.read());
  }
}