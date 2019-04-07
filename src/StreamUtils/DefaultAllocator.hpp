// StreamUtils - github.com/bblanchon/StreamUtils
// Copyright Benoit Blanchon 2019
// MIT License

namespace StreamUtils {

#include <stdlib.h>  // malloc, free, size_t

struct DefaultAllocator {
  void* allocate(size_t n) {
    return malloc(n);
  }

  void deallocate(void* p) {
    free(p);
  }
};

}  // namespace StreamUtils