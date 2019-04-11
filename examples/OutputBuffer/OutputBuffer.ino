// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License
//
// This example shows how to buffer the output of a stream.
//
// Like "echo," it reads from the serial port and prints back the same thing.
// What's interesting with this program is that it writes in chunks of
// 8 bytes, even if it seems to write them one by one.
//
// As you'll see, you need to type at least 8 bytes to see something. That's
// because it waits for the buffered to be full before sending it to the serial.

#include <StreamUtils.h>

auto bufferedSerial = bufferizeOutput(Serial, 8);

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) continue;

  Serial.println(F("Send at least 8 bytes to see the result"));
}

void loop() {
  // Even if it looks like the bytes are extracted one by one, they are actual
  // read by chunks in 64 bytes and placed in a buffer.
  while (Serial.available()) {
    bufferedSerial.write(Serial.read());
  }
}