#include "errorfn/errorfn.h"
#include "framebuffer.h"

// quad that fills the screen
const float quadVerts[18] = {
  -1.0, 1.0, 0.0, // top left
  1.0, 1.0, 0.0, // top right
  -1.0, -1.0, 0.0, // bottom left

  -1.0, -1.0, 0.0, // bottom left
  1.0, -1.0, 0.0, // bottom right
  1.0, 1.0, 0.0 // top right
};

ErrorFn::ErrorFn(Texture & target, Texture &canvas)
  : fTarget(target),
            fCanvas(canvas) {  
  // create another framebuffer the same size as the target image
  // texture filtering is disabled, see framebuffer.cpp:
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // this is required for the error function to work properly
  int w = target.GetWidth();
  int h = target.GetHeight();
  fPixelDifferences = FramebufferTexture(target.GetWidth(), target.GetHeight());

  // if the image is wider than it is tall, summing the rows into a
  // single column will be better
  fSumColumns = (w > h);
  int shortestAxis;
  if (fSumColumns) {
    shortestAxis = h;
  } else {
    shortestAxis = w;
  }

  // create second framebuffer to act as the shortest axis array
  fSummed = FramebufferTexture(shortestAxis, 1);

  // TODO: create a shader that computes the distance between the two
  // images and outputs to another texture

  // TODO: create a shader that sums distances to a single row
}
