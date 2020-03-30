// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2020
// MIT License

#pragma once

#include <string>

class String : private std::string {
 public:
  String() {}
  String(const String& s) : std::string(s) {}
  String(String&& s) : std::string(std::move(s)) {}
  String(const char* s) : std::string(s) {}
  String(int n) : std::string(std::to_string(n)) {}

  String& operator=(const String& rhs) {
    std::string::operator=(rhs);
    return *this;
  }

  using std::string::c_str;
  using std::string::length;
  using std::string::operator+=;
  using std::string::operator[];

  void remove(unsigned int index, unsigned int count) {
    erase(begin() + index, begin() + index + count);
  }

  void toCharArray(char* buf, unsigned int bufsize,
                   unsigned int index = 0) const {
    copy(buf, bufsize, index);
  }

  friend bool operator==(const String& lhs, const char* rhs) {
    return static_cast<const std::string&>(lhs) == rhs;
  }

  friend std::ostream& operator<<(std::ostream& lhs, const String& rhs) {
    return lhs << static_cast<const std::string&>(rhs);
  }
};
