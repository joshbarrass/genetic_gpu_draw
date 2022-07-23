#include "images/image.h"
#include <stb/stb_image.h>
#include <glad/glad.h>

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

Image::Image(const FramebufferTexture fbtex) {
  fWidth = fbtex.GetWidth();
  fHeight = fbtex.GetHeight();
  fImageData = new GLubyte[3*fWidth*fHeight];

  // ensure that the images are not aligned to any boundaries
  // this may introduce padding, which will corrupt the buffer
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, fbtex.GetTex());
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, fImageData);

  fUsedStb = false;
  FlipVerticallyOnWrite = true;
}

Image::Image(const FramebufferCache &fbcache) {
  fWidth = fbcache.GetWidth();
  fHeight = fbcache.GetHeight();
  fImageData = new GLubyte[3*fWidth*fHeight];

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, fbcache.GetTex());
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, fImageData);

  fUsedStb = false;
  FlipVerticallyOnWrite = true;
}

// convert an x/y corrd to an array index based on the width of the image
constexpr int get_array_index(int x, int y, int w) {
  return w*y + x;
}

unsigned char Image::GetPixelValue(int x, int y) const {
  return fImageData[get_array_index(x, y, fWidth)];
}
