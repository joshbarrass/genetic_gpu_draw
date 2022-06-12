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
  double GetError();

#ifndef BUILD_DEBUG
private:
#endif
  GLuint GetDiffTexID() const { return fPixelDifferences.GetTex(); }
  GLuint GetSumTexID() const { return fSummed.GetTex(); }
  void Run() { RunDifferenceShader(); RunSummationShader(); }

#ifdef BUILD_DEBUG
private:
#endif
  void DrawQuad();
  void RunDifferenceShader();
  void RunSummationShader();
  
  ///////////
  
  Texture &fTarget;
  FramebufferTexture &fCanvas;

  GLuint VAO;
  GLuint VBO;

  FramebufferTexture fPixelDifferences; // used to calculate the difference for each pixel
  bool fSumToColumn;
  FramebufferTexture fSummed; // sum all rows/columns into a single array

  Shader fDifferenceShader; // shader to compute the difference between each pixel
  Shader fSummationShader; // shader to sum over one axis
};

#endif
