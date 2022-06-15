#ifndef FRAMEBUFFER_CACHE_H
#define FRAMEBUFFER_CACHE_H 1

#include "framebuffer.h"
#include "shaderClass.h"
#include <glad/glad.h>

class FramebufferCache {
public:
  FramebufferCache(FramebufferTexture&);
  ~FramebufferCache();

  int GetWidth() const { return fFramebuffer.GetWidth(); }
  int GetHeight() const { return fFramebuffer.GetHeight(); }
  GLuint GetTex() const { return Tex; }

  void Cache();
  void Restore();
private:
  void DrawQuad();
  
  FramebufferTexture &fFramebuffer;
  GLuint Tex;

  // quad
  GLuint VAO;
  GLuint VBO;

  Shader fPassthroughShader;
};

#endif
