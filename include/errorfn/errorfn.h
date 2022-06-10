#ifndef ERROR_FN_H
#define ERROR_FN_H 1

#include "textureClass.h"
#include "framebuffer.h"
#include "shaderClass.h"

class ErrorFn {
public:
  ErrorFn(Texture &target, Texture &canvas);
  ~ErrorFn();
private:
  Texture &fTarget;
  Texture &fCanvas;

  FramebufferTexture fPixelDifferences; // used to calculate the difference for each pixel
  bool fSumColumns;
  FramebufferTexture fSummed; // sum all rows/columns into a single array

  Shader fDifferenceShader; // shader to compute the difference between each pixel
};

#endif
