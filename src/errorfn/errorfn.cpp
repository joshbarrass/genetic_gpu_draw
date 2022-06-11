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

// TODO:
ErrorFn::~ErrorFn() {}

ErrorFn::ErrorFn(Texture & target, FramebufferTexture &canvas)
  : fTarget(target),
    fCanvas(canvas), fPixelDifferences(), fSummed(), fDifferenceShader("shaders/simpleVertShader.glsl", "shaders/difference_shader.glsl") {
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
  // must use GL_RGBA32F here to get sufficient precision and to
  // bypass clamping
  fPixelDifferences = FramebufferTexture(target.GetWidth(), target.GetHeight(), GL_RGBA32F);

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
  fSummed = FramebufferTexture(shortestAxis, 1, GL_RGBA32F);

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
  // bind the textures
  const GLuint targetUnitNumber = 0;
  const GLuint canvasUnitNumber = 1;
  fTarget.Use(targetUnitNumber);
  // bind canvas texture manually
  glActiveTexture(GL_TEXTURE0 + canvasUnitNumber);
  glBindTexture(GL_TEXTURE_2D, fCanvas.GetTex());
  fDifferenceShader.setInt("target", targetUnitNumber);
  fDifferenceShader.setInt("canvas", canvasUnitNumber);
  fDifferenceShader.set2Vec("screen_size", this->GetWidth(), this->GetHeight());

  DrawQuad();
  // each pixel in framebuffer should now be the difference
}
