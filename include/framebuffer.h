#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H 1

#include <glad/glad.h>

constexpr int ERR_COULD_NOT_MAKE_FRAMEBUFFER = 10;

class FramebufferTexture {
public:
  FramebufferTexture(int width, int height, GLuint texFormat=GL_RGBA);
  FramebufferTexture();
  ~FramebufferTexture();

  void Use();
  int GetWidth() const { return fWidth; }
  int GetHeight() const { return fHeight; }
  GLuint GetTex() const { return Tex; }

private:
  int fWidth;
  int fHeight;
  
  GLuint FBO;
  GLuint Tex;
  GLuint DepthBuf;
  GLenum DrawBuffers[1];
};

#endif
