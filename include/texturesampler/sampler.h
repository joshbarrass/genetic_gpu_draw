// -*- C++ -*-
#ifndef TEXTURE_SAMPLER_H
#define TEXTURE_SAMPLER_H 1

#include "textureClass.h"
#include "framebuffer.h"
#include "shaderClass.h"

class TextureSampler {
public:
  TextureSampler(Texture &target);
  ~TextureSampler();

  void SampleTexture(float x, float y, GLfloat &r, GLfloat &g, GLfloat &b);

  GLuint GetSamplerTexID() const { return fSampleBuffer.GetTex(); }

private:
  void DrawQuad();
  void RunSampleShader(float, float);
  void GetBufferColour(GLfloat &r, GLfloat &g, GLfloat &b);
  
  Texture &fTarget;

  GLuint VAO;
  GLuint VBO;

  FramebufferTexture fSampleBuffer;
  Shader fSampleShader;
};

#endif
