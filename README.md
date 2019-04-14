StreamUtils: Power-ups for Arduino Streams
==========================================

The *stream* is an essential abstraction of Arduino, we find it in many places:

* `HardwareSerial`
* `SoftwareSerial`
* `File`
* `EthernetClient`
* `WiFiClient`
* and many others...

This library provides some helpers classes and functions for dealing with streams. 

For example, with this library, you can improve the performance of your program by buffering the data it reads from a file. You could also debug your program more easily by logging what it sends to a Web service.

Read on, to see how StreamUtils can help your program.

Buffering read operations
-------------------------

Sometimes, you can greatly improve performance by reading many bytes at once. 
For example, [according to SPIFFS's wiki](https://github.com/pellepl/spiffs/wiki/Performance-and-Optimizing#reading-files), it's much faster to read files in chunks of 64 bytes than reading them one byte at a time.

![Input buffer](examples/ReadBuffer/ReadBuffer.svg)

To buffer the input, simply decorate the original `Stream` with `ReadBufferingStream`. For example, suppose your program reads a JSON document from SPIFFS, like that:

```c++
File file = SPIFFS.open("example.json", "r");
deserializeJson(doc, file);
```

Then you simply need to insert one line to greatly improve the reading speed:

```c++
File file = SPIFFS.open("example.json", "r");
ReadBufferingStream bufferedSerial{Serial, 64};  // <- HERE
deserializeJson(doc, bufferedFile);
```

Unfortunately, this optimization is only possible if:

1. `Stream.readBytes()` is declared `virtual` in your Arduino Code (as it's the case for ESP8266), and
2. the derived class has an optimized implementation of `readBytes()` (as it's the case for SPIFFS' `File`).

Buffering write operations
--------------------------

Similarly, you can greatly improve performance by writing many bytes at once.
For example, if you write to `WiFiClient` one bytes at a time, it will be very slow; it's much faster if you send large chunks.

![Output buffer](examples/WriteBuffer/WriteBuffer.svg)

To add a buffer, decorate the original `Stream` with  `WriteBufferingStream`. For example, if you program send a JSON document via `WiFiClient`, like that:

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


Logging what's written to a Stream
----------------------------------

When debugging a program that makes HTTP request, the first thing you want to check is that the request is correct. With this library you can decorate the `EthernetStream` or the `WiFiStream` to log everything to the serial.

![Output logger](examples/OutputLogger/OutputLogger.svg)

For example, if you program is:

```c++
client.println("GET / HTTP/1.1");
client.println("User-Agent: Arduino");
// ...
```

Then, call `logOuput()` to decorate the original stream:

```c++
auto loggingClient = logOutput(client, Serial);
loggingClient.println("GET / HTTP/1.1");
loggingClient.println("User-Agent: Arduino");
// ...
```

Everything you write to `loggingClient` is written to `client` and logged to `Serial`.

