#include "framebuffer_cache.h"
#include "framebuffer.h"
#include "consts.h"

#include <filesystem>
#include "exepath.h"
 
// TODO: create a single shared quad for both the cache and the error
// function?
// quad that fills the screen
const float quadVerts[] = {
  -1.0, 1.0, // top left
  1.0, 1.0, // top right
  -1.0, -1.0, // bottom left

  -1.0, -1.0, // bottom left
  1.0, -1.0, // bottom right
  1.0, 1.0 // top right
};

FramebufferCache::FramebufferCache(FramebufferTexture &to_cache) : fFramebuffer(to_cache), fPassthroughShader() {
  // load shaders
  char exepathBuf[256];
  const size_t bufLen = sizeof(exepathBuf);
  getExecutablePath(exepathBuf, bufLen);
  const std::filesystem::path exepath(exepathBuf);
  const std::filesystem::path exedir = exepath.parent_path();
  fPassthroughShader = Shader((exedir / "shaders/simpleVertShader.glsl").c_str(), (exedir / "shaders/passthroughFragShader.glsl").c_str());
  
  // create a new texture
  glGenTextures(1, &Tex);
  glBindTexture(GL_TEXTURE_2D, Tex);

  // set up wrapping and disable filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // create VBO and VAO
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), &quadVerts[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}

// TODO:
FramebufferCache::~FramebufferCache() {}

void FramebufferCache::Cache() {
  // activate the framebuffer
  fFramebuffer.Use();

  // activate the texture
  glBindTexture(GL_TEXTURE_2D, Tex);

  // copy the contents of the framebuffer into the texture
  glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, fFramebuffer.GetWidth(), fFramebuffer.GetHeight(), 0);
}

void FramebufferCache::Restore() {
  // activate the framebuffer
  fFramebuffer.Use();

  // enable the shader
  fPassthroughShader.use();
  // bind the texture
  const GLuint cacheUnitNumber = cacheImageUnitNumber;
  // bind texture manually
  glActiveTexture(GL_TEXTURE0 + cacheUnitNumber);
  glBindTexture(GL_TEXTURE_2D, Tex);
  fPassthroughShader.setInt("target_image", cacheUnitNumber);
  fPassthroughShader.set2Vec("image_size", fFramebuffer.GetWidth(), fFramebuffer.GetHeight());

  DrawQuad();
}

void FramebufferCache::DrawQuad() {
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glDrawArrays(GL_TRIANGLES, 0, 3*2 /* 2 triangles of 3 vertices each */);
}
