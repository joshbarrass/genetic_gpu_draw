#include "images/image.h"
#include <stb/stb_image.h>

Image::~Image() {
  // images loaded via stb should be freed with stb
  // raw loading should just be freed
  if (fUsedStb) {
    stbi_image_free(fImageData);
  } else {
    delete[] fImageData;
  }
}

void Image::construct(const char *filename) {
  int _;
  fImageData = stbi_load(filename, &fWidth, &fHeight, &_, 3);
  fUsedStb = true;
  FlipVerticallyOnWrite = false;
}

Image::Image(const char *filename) {
  construct(filename);
}

Image::Image(const std::string filename) {
  construct(filename.c_str());
}

Image::Image(const std::filesystem::path filename) {
  construct(filename.u8string().c_str());
}

Image::Image(GLFWwindow *window) {
  glfwGetWindowSize(window, &fWidth, &fHeight);
  fImageData = new GLubyte[3*fWidth*fHeight];
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(0, 0, fWidth, fHeight, GL_RGB, GL_UNSIGNED_BYTE, fImageData);

  fUsedStb = false;
  FlipVerticallyOnWrite = true;
}
