// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2020
// MIT License
//
// This example shows how to buffer the read operations of a stream.
//
// Like "echo," it reads from the serial port and prints back the same thing.
// What's interesting with this program is that it reads the input in chunks of
// 64 bytes, even if it seems to read them one by one.
//
// As you'll see, there is a one-second delay between the moment you press
// [Enter] and the moment the content appears in the Serial Monitor.
// Indeed, fewer than 64 bytes are available, so Serial waits for a timeout.
//
// If you press [Enter] several times within a second, you'll observe that
// everything appears at the same time. That's because the buffer aggregated all
// the data before the timeout.

#include <StreamUtils.h>

ReadBufferingStream bufferedSerial{Serial, 64};

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial)
    continue;
}

void loop() {
  // Even if it looks like the bytes are extracted one by one, they are actual
  // read by chunks in 64 bytes and placed in a buffer.
  while (bufferedSerial.available()) {
    Serial.write(bufferedSerial.read());
  }
}

/*****************************************************
 *                                                   *
 *  Love this project?                               *
 *               Star it on GitHub!                  *
 *                                                   *
 *                       .,,.                        *
 *                       ,,:1.                       *
 *                      ,.,:;1                       *
 *                     .,,,::;:                      *
 *                     ,,,,::;;.                     *
 *                    .,,,:::;;;                     *
 *       .....,,,,...,.,,,,,,:::,,,,,,,,,,,,,        *
 *      ,,,,,,,,,,,:,...,,,,,,:::,,,,:::;;;11l       *
 *       .;::::::::,,,,,,,,,,:::::,,::;;;1lt         *
 *          .;;;:::,,,,,,,,::::::;:::;;1t:           *
 *             :;;:,,,,,,::::::;;;;;;l1              *
 *                ,,,,:::::::;;;;;;l                 *
 *               .,,,,::::;;;;;;;::::                *
 *               ,,,,,:::;;;;;::,:::1                *
 *              ,,,,,::;;;t1:,,:::::;l               *
 *             .,,,,:;;ll    ;::::::;;,              *
 *             ,,,:;ll.        .1:::;;l              *
 *            .,:lt,              .1;;l:             *
 *                                                   *
 *  https://github.com/bblanchon/ArduinoStreamUtils  *
 *                                                   *
 *****************************************************/