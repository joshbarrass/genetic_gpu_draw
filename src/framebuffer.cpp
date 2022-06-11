#include "framebuffer.h"
#include <stdexcept>

// do nothing; used as a null initialiser
FramebufferTexture::FramebufferTexture() : fWidth(0), fHeight(0) {}

// will also bind the new framebuffer!
FramebufferTexture::FramebufferTexture(int width, int height, GLuint texFormat) : fWidth(width), fHeight(height) {
  // create and bind the new framebuffer
  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  // create and bind the texture
  glGenTextures(1, &Tex);
  glBindTexture(GL_TEXTURE_2D, Tex);

  // create an empty image
  glTexImage2D(GL_TEXTURE_2D, 0, texFormat, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

  // reduce filtering (required)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // create depth buffer
  glGenRenderbuffers(1, &DepthBuf);
  glBindRenderbuffer(GL_RENDERBUFFER, DepthBuf);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthBuf);

  // Set "renderedTexture" as our colour attachment #0
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Tex, 0);

  // Set the list of draw buffers.
  DrawBuffers[0] = GL_COLOR_ATTACHMENT0;
  glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw ERR_COULD_NOT_MAKE_FRAMEBUFFER;
  }
}

// TODO:
FramebufferTexture::~FramebufferTexture() {}

void FramebufferTexture::Use() {
  // Render to our framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glViewport(0, 0, fWidth, fHeight);
}
