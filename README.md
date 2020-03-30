StreamUtils: Power-ups for Arduino Streams
==========================================

[![arduino-library-badge](https://www.ardu-badge.com/badge/StreamUtils.svg?version=1.4.0)](https://www.ardu-badge.com/StreamUtils/1.4.0)
[![Build Status](https://travis-ci.org/bblanchon/ArduinoStreamUtils.svg?branch=master)](https://travis-ci.org/bblanchon/ArduinoStreamUtils)

The *stream* is an essential abstraction in Arduino, we find it in many places:

* `HardwareSerial`
* `SoftwareSerial`
* `File`
* `EthernetClient`
* `WiFiClient`
* and many others...

This library provides some helpers classes and functions for dealing with streams. 

For example, with this library, you can improve the performance of your program by buffering the data it reads from a file. You could also debug your program more easily by logging what it sends to a Web service.

Read on to see how StreamUtils can help your program!

Buffering read operations
-------------------------

Sometimes, you can significantly improve performance by reading many bytes at once. 
For example, [according to SPIFFS's wiki](https://github.com/pellepl/spiffs/wiki/Performance-and-Optimizing#reading-files), it's much faster to read files in chunks of 64 bytes than reading them one byte at a time.

![ReadBufferingStream](extras/images/ReadBuffer.svg)

To buffer the input, simply decorate the original `Stream` with `ReadBufferingStream`. For example, suppose your program reads a JSON document from SPIFFS, like that:

```c++
File file = SPIFFS.open("example.json", "r");
deserializeJson(doc, file);
```

Then you simply need to insert one line to greatly improve the reading speed:

```c++
File file = SPIFFS.open("example.json", "r");
ReadBufferingStream bufferedFile{file, 64};  // <- HERE
deserializeJson(doc, bufferedFile);
```

Unfortunately, this optimization is only possible if:

1. `Stream.readBytes()` is declared `virtual` in your Arduino Code (as it's the case for ESP8266), and
2. the derived class has an optimized implementation of `readBytes()` (as it's the case for SPIFFS' `File`).

When possible, prefer `ReadBufferingClient` to `ReadBufferingStream` because `Client` defines a `read()` method similar to `readBytes()`, except that this one is `virtual` on all platforms.

Buffering write operations
--------------------------

Similarly, you can improve performance significantly by writing many bytes at once.
For example, if you write to `WiFiClient` one bytes at a time, it will be very slow; it's much faster if you send large chunks.

![WriteBufferingStream](extras/images/WriteBuffer.svg)

To add a buffer, decorate the original `Stream` with  `WriteBufferingStream`. For example, if your program sends a JSON document via `WiFiClient`, like that:

```c++
serializeJson(doc, wifiClient);
```

Then, you just need to add two lines:

```c++
WriteBufferingStream bufferedWifiClient{wifiClient, 64};
serializeJson(doc, bufferedWifiClient);
bufferedWifiClient.flush();  // <- OPTIONAL
```

Calling `flush()` is recommended but not mandatory. If you don't call it, the destructor of `WriteBufferingStream` will do it for you.


Logging write operations
------------------------

When debugging a program that makes HTTP requests, the first thing you want to check is that the request is correct. With this library, you can decorate the `EthernetClient` or the `WiFiClient` to log everything to the serial.

![WriteLoggingStream](extras/images/WriteLogger.svg)

For example, if you program is:

```c++
client.println("GET / HTTP/1.1");
client.println("User-Agent: Arduino");
// ...
```

Then, you just need to create the decorator, and update the calls to `println()`:

```c++
WriteLoggingStream loggingClient(client, Serial);
loggingClient.println("GET / HTTP/1.1");
loggingClient.println("User-Agent: Arduino");
// ...
```

Everything you write to `loggingClient` is written to `client` and logged to `Serial`.

Logging read operations
-----------------------

Similarly, you often want to see what the HTTP server sent back. With this library, you can decorate the `EthernetClient` or the `WiFiClient` to log everything to the serial.

![ReadLoggingStream](extras/images/ReadLogger.svg)

For example, if you program is:

```c++
char response[256];
client.readBytes(response, 256);
```

Then, you just need to create the decorator, and update the calls to `readBytes()`:

```c++
ReadLoggingStream loggingClient(client, Serial);
char response[256];
loggingClient.readBytes(response, 256);
// ...
```

`loggingClient` forwards all operations to `client` and logs read operation to `Serial`.

Logging read and write operations
---------------------------------

Of course, you could log read and write operations by combining `ReadLoggingStream` and `WriteLoggingStream`, but there is a simpler solution: `LoggingStream`.

![LoggingStream](extras/images/Logger.svg)

As usual, if your program is:

```c++
client.println("GET / HTTP/1.1");
client.println("User-Agent: Arduino");

char response[256];
client.readBytes(response, 256);
```

Then decorate `client` and replace the calls:

```c++
LoggingStream loggingClient(client, Serial);

loggingClient.println("GET / HTTP/1.1");
loggingClient.println("User-Agent: Arduino");

char response[256];
loggingClient.readBytes(response, 256);
```


Writing to a `String`
---------------------

Sometimes, you use a piece of code that expects a `Print` instance (like `ReadLoggingStream`), but you want the output in a `String` instead of a regular `Stream`.
In that case, use the `StringPrint` class. It wraps a `String` within a `Print` implementation.

![StringPrint](extras/images/StringPrint.svg)

Here is how you can use it:

```c++
StringPrint stream;

stream.print("Temperature = ");
stream.print(22.3);
stream.print(" °C");

String result = stream.str();
```

At the end of this snippet, the string `result` contains:

```
Temperature = 22.30 °C
```


Reading from a `String`
-----------------------

Similarly, there are cases where you have a `String`, but you need to pass a `Stream` to some other piece of code. In that case, use `StringStream`; it's similar to `StrintPrint`, except you can read as well.

![StringStream](extras/images/StringStream.svg)


EEPROM as a stream
------------------

SteamUtils also allows using EEPROM as a stream. Simply create an instance of `EepromStream` and specify the start address and the size of the region you want to expose.

![EepromStream](extras/images/EepromStream.svg)

For example, it allows you to save a JSON document in EEPROM:

```c++
EepromStream eepromStream(0, 128);
serializeJson(doc, eepromStream);
eepromStream.flush();  // <- calls EEPROM.commit() on ESP (optional)
```

In the same way, you can read a JSON document from EEPROM:

```c++
EepromStream eepromStream(0, 128);
deserializeJson(doc, eepromStream);
```

Other classes
-------------

Some of the decorators are also available for the `Print` and `Client` classes.
See the equivalence table below.

| Purpose                    | `Client`               | `Stream`               | `Print`          |
|:---------------------------|:-----------------------|:-----------------------|:-----------------|
| Log *write* operations     | `WriteLoggingClient`   | `WriteLoggingStream`   | `LoggingPrint`   |
| Log *read* operations      | `ReadLoggingClient`    | `ReadLoggingStream`    |                  |
| Log *read* and *write* op. | `LoggingClient`        | `LoggingStream`        |                  |
| Buffer *write* operations  | `WriteBufferingClient` | `WriteBufferingStream` | `BufferingPrint` |
| Buffer *read* operations   | `ReadBufferingClient`  | `ReadBufferingStream`  |                  |
| Use `String` as a stream   |                        | `StringStream`         | `StringPrint`    |
| Use EEPROM as a stream     |                        | `EepromStream`         |                  |

When possible, prefer `ReadBufferingClient` to `ReadBufferingStream` because `Client::read()` often provides an optimized implementation.


Portability
-----------

This library relies on the definitions of `Client`, `Print`, and `Stream`, which unfortunately differ from one core to another.

It has been tested on the following cores:

* AVR
* ESP32
* ESP8266
* SAMD
* Teensy

If your core is not supported, please [open an issue](https://github.com/bblanchon/ArduinoStreamUtils/issues/new).
Thank you for your understanding.
