#include "errorfn/errorfn.h"
#include "framebuffer.h"

// quad that fills the screen
const float quadVerts[] = {
  -1.0, 1.0, // top left
  1.0, 1.0, // top right
  -1.0, -1.0, // bottom left

  -1.0, -1.0, // bottom left
  1.0, -1.0, // bottom right
  1.0, 1.0 // top right
};

ErrorFn::ErrorFn(Texture & target, Texture &canvas)
  : fTarget(target),
    fCanvas(canvas), fPixelDifferences(), fSummed(), fDifferenceShader("shaders/simpleVertShader.glsl", "shaders/summing_shader.glsl") {
  // create VBO and VAO
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), &quadVerts[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
  
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

  // TODO: create a shader that sums distances to a single row
}

void ErrorFn::DrawQuad() {
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glDrawArrays(GL_TRIANGLES, 0, 3*2 /* 2 triangles of 3 vertices each */);
}

void ErrorFn::RunDifferenceShader() {
  // bind the difference framebuffer
  fPixelDifferences.Use();

  // enable the shader
  fDifferenceShader.use();
  fDifferenceShader.setInt("target", fTarget.GetID());
  fDifferenceShader.setInt("canvas", fCanvas.GetID());

  DrawQuad();
  // each pixel in framebuffer should now be the difference
}
