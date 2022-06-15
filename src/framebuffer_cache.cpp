#include "framebuffer_cache.h"
#include "framebuffer.h"

FramebufferCache::FramebufferCache(FramebufferTexture &to_cache) : fFramebuffer(to_cache) {
  // create a new texture
  glGenTextures(1, &Tex);
  glBindTexture(GL_TEXTURE_2D, Tex);

  // set up wrapping and disable filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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

