#ifndef IMAGES_IMAGE_H
#define IMAGES_IMAGE_H 1

#include <filesystem>
#include <string>
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Image {
public:
  Image(const std::filesystem::path filename);
  Image(const std::string filename);
  Image(const char *filename);
  Image(GLFWwindow *window);
  ~Image();

  void Save(const char *filename);
  void Save(const std::string filename);
  void Save(const std::filesystem::path filename);

  int GetWidth() { return fWidth; }
  int GetHeight() { return fHeight; }

  // used for saving, but could be overridden
  bool FlipVerticallyOnWrite;
private:
  void construct(const char *filename);
  void save_image(const char *filename);

  // used on destruction for proper cleanup
  bool fUsedStb;
  
  unsigned char *fImageData;
  int fWidth;
  int fHeight;
};

#endif
