// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include "Stream.h"

#include <string>

using IPAddress = std::string;

class Client : public Stream {
 public:
  virtual int connect(const IPAddress &ip, uint16_t port) = 0;
  virtual int connect(const char *host, uint16_t port) = 0;
  virtual size_t write(uint8_t) = 0;
  virtual size_t write(const uint8_t *buf, size_t size) = 0;
  virtual int available() = 0;
  virtual int read() = 0;
  virtual int read(uint8_t *buf, size_t size) = 0;
  virtual int peek() = 0;
  virtual bool flush(unsigned int maxWaitMs = 0) = 0;
  virtual bool stop(unsigned int maxWaitMs = 0) = 0;
  virtual uint8_t connected() = 0;
  virtual operator bool() = 0;
};
