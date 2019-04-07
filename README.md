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

To buffer the input, simply pass your stream to `bufferizeInput()`, and it will return a new stream with an internal buffer.

For example, suppose you program reads a JSON document from SPIFFS, like that:

```c++
File file = SPIFFS.open("example.json", "r");
deserializeJson(doc, file);
```

Then you simply need to insert one line to greatly improve the reading speed:

```c++
File slowFile = SPIFFS.open("example.json", "r");
auto fastFile = bufferizeInput(slowFile, 64); // <- HERE
deserializeJson(doc, fastFile);
```

Unfortunately, this optimization is only possible if:

1. `Stream.readBytes()` is declared `virtual` in your Arduino Code (as it's the case for ESP8266), and
2. the derived class has an optimized implementation of `readBytes()` (as it's the case for SPIFFS' `File`).
