#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H 1

#include <glad/glad.h>

constexpr int ERR_COULD_NOT_MAKE_FRAMEBUFFER = 10;

class FramebufferTexture {
public:
  FramebufferTexture(int, int);
  ~FramebufferTexture();

  void Use();
private:
  int fWidth;
  int fHeight;
  
  GLuint FBO;
  GLuint Tex;
  GLuint DepthBuf;
  GLenum DrawBuffers[1];
};

#endif
