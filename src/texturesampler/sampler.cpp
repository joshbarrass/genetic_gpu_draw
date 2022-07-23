#include "texturesampler/sampler.h"
#include "consts.h"

// quad that fills the screen
const float quadVerts[] = {
  -1.0, 1.0, // top left
  1.0, 1.0, // top right
  -1.0, -1.0, // bottom left

  -1.0, -1.0, // bottom left
  1.0, -1.0, // bottom right
  1.0, 1.0 // top right
};

TextureSampler::~TextureSampler() {
  // delete VAO and VBO
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  // other objects should go out of scope
}

TextureSampler::TextureSampler(Texture &target) : fTarget(target), fSampleBuffer(1, 1, GL_RGB), fSampleShader("shaders/simpleVertShader.glsl", "shaders/sample_shader.glsl") {
  // create VBO and VAO
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), &quadVerts[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  // single pixel framebuffer for sampling already created
  // texture filtering is disabled in framebuffer.cpp, which is essential
}

void TextureSampler::DrawQuad() {
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glDrawArrays(GL_TRIANGLES, 0, 3*2);
}

void TextureSampler::SampleTexture(float x, float y, GLfloat &r, GLfloat &g, GLfloat &b) {
  RunSampleShader(x, y);
  GetBufferColour(r, g, b);
}

void TextureSampler::RunSampleShader(float x, float y) {
  // bind framebuffer
  fSampleBuffer.Use();

  // enable the shader
  fSampleShader.use();
  // bind the texture
  const GLuint targetUnitNumber = samplerImageUnitNumber;
  fTarget.Use(targetUnitNumber);
  // set the uniforms
  fSampleShader.setInt("target", targetUnitNumber);
  fSampleShader.set2Vec("sample_coord", x, y);
  fSampleShader.set2Vec("screen_size", fTarget.GetWidth(), fTarget.GetHeight());

  DrawQuad();
  // framebuffer should have the correct colour in it
}

void TextureSampler::GetBufferColour(GLfloat &r, GLfloat &g, GLfloat &b) {
  // dump the contents of the buffer
  GLuint texID = GetSamplerTexID();
  GLfloat imageData[3];
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texID);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, imageData);
  r = imageData[0];
  g = imageData[1];
  b = imageData[2];
}
