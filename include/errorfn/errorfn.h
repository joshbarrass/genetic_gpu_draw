#ifndef ERROR_FN_H
#define ERROR_FN_H 1

#include "textureClass.h"
#include "framebuffer.h"
#include "shaderClass.h"

class ErrorFn {
public:
  ErrorFn(Texture &target, FramebufferTexture &canvas);
  ~ErrorFn();

  int GetWidth() const { return fTarget.GetWidth(); }
  int GetHeight() const { return fTarget.GetHeight(); }
  GLuint GetDiffTexID() const { return fPixelDifferences.GetTex(); }

  void Run() { RunDifferenceShader(); } // TODO: remove

private:
  void DrawQuad();
  void RunDifferenceShader();
  
  ///////////
  
  Texture &fTarget;
  FramebufferTexture &fCanvas;

  GLuint VAO;
  GLuint VBO;

  FramebufferTexture fPixelDifferences; // used to calculate the difference for each pixel
  bool fSumColumns;
  FramebufferTexture fSummed; // sum all rows/columns into a single array

  Shader fDifferenceShader; // shader to compute the difference between each pixel
};

#endif
