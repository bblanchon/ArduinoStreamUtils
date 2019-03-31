Arduino Stream Utilities
========================

A collection of helpers for Arduino Stream.

1. Buffered input
-----------------

Improve read performance by reading chunks instead of reading bytes one by one.

```c++
auto bufferedStream = bufferizeInput(file);
deserializeJson(doc, bufferedStream);
```

NOTE: this only makes sense if the upstream class has an optimize `readBytes()` implementation, like SPIFFS's `File`.
