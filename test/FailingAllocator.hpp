// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

#pragma once

#include <stdlib.h>  // size_t

struct FailingAllocator {
  void* allocate(size_t) {
    return nullptr;
  }
  void deallocate(void*) {}
};
