#include "images/save.h"
#include <GLFW/glfw3.h>
#include <stb/stb_image_write.h>

void save_image(GLFWwindow *window, const char *fn) {
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  GLubyte *pixels = new GLubyte[3*width*height];
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  stbi_flip_vertically_on_write(true);
  stbi_write_png(fn, width, height, 3, pixels, 3*width);
}
