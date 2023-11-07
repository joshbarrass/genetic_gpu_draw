#include "exepath.h"

int MIN(int a, int b) {
  if (a > b) {
    return b;
  }
  return a;
}

#ifdef __unix__
#include <unistd.h>

// writes the executable dir to the provided buffer (up to the length
// of the buffer) and returns the number of characters (not including
// the null terminator)
size_t getExecutablePath(char *buf, size_t len) {
  int bytes = MIN(readlink("/proc/self/exe", buf, len), len - 1);
  if(bytes >= 0)
    buf[bytes] = '\0';
  return bytes;
}

#elif defined(_WIN32) || defined(WIN32)
#include <Windows.h>

size_t getExecutablePath(char *buf, size_t len) {
  int bytes = GetModuleFileName(NULL, buf, len);
  return bytes ? bytes : -1;
}

#endif
