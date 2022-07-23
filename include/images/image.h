#ifndef IMAGES_IMAGE_H
#define IMAGES_IMAGE_H 1

#include <filesystem>
#include <string>
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "framebuffer.h"
#include "framebuffer_cache.h"

class Image {
public:
  Image(const std::filesystem::path filename);
  Image(const std::string filename);
  Image(const char *filename);
  Image(GLFWwindow *window);
  Image(const FramebufferTexture);
  Image(const FramebufferCache&);
  ~Image();

  void Save(const char *filename) const;
  void Save(const std::string filename) const;
  void Save(const std::filesystem::path filename) const;

  int GetWidth() const { return fWidth; }
  int GetHeight() const { return fHeight; }

  // used for saving, but could be overridden
  bool FlipVerticallyOnWrite;
private:
  void construct(const char *filename);
  void save_image(const char *filename) const;

  // used on destruction for proper cleanup
  bool fUsedStb;
  
  unsigned char *fImageData;
  int fWidth;
  int fHeight;
};

#endif
