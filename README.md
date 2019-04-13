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

Buffering the input of a stream
-------------------------------

Sometimes, you can greatly improve performance by reading many bytes at once. 
For example, [according to SPIFFS's wiki](https://github.com/pellepl/spiffs/wiki/Performance-and-Optimizing#reading-files), it's much faster to read files in chunks of 64 bytes than reading them one byte at a time.

To buffer the input, simply pass your stream to `bufferInput()`, and it will return a new stream with an internal buffer.

![Input buffer](examples/InputBuffer/InputBuffer.svg)

For example, suppose your program reads a JSON document from SPIFFS, like that:

```c++
File file = SPIFFS.open("example.json", "r");
deserializeJson(doc, file);
```

Then you simply need to insert one line to greatly improve the reading speed:

```c++
File file = SPIFFS.open("example.json", "r");
auto bufferedFile = bufferInput(file, 64); // <- HERE
deserializeJson(doc, bufferedFile);
```

Unfortunately, this optimization is only possible if:

1. `Stream.readBytes()` is declared `virtual` in your Arduino Code (as it's the case for ESP8266), and
2. the derived class has an optimized implementation of `readBytes()` (as it's the case for SPIFFS' `File`).

Buffering the output of a stream
--------------------------------

Similarly, you can greatly improve performance by writing many bytes at once.
For example, if you write to `WiFiClient` one bytes at a time, it will be very slow; it's much faster if you send large chunks.

To buffer the output of a stream, pass it to `bufferOutput()`, and you'll receive a new stream with an internal buffer.

![Output buffer](examples/OutputBuffer/OutputBuffer.svg)

For example, if you program send a JSON document via `WiFiClient`, like that:

```c++
serializeJson(doc, wifiClient);
```

Then, you just need to add two lines:

```c++

```

Calling `flush()` is recommended but not mandatory. If you don't call it, the destructor of `StreamWithOutputBuffer` (the class of `bufferedWifiClient`) will do it for you.

