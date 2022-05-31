#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H 1

#include <glad/glad.h>

class Texture {
private:
  GLsizei width, height;
  GLint nChannels;
  GLuint ID;
  bool haveGivenUnitWarning;
public:
  Texture(const char* texturePath, const GLenum srcColorScheme, const GLenum destColorScheme);
  GLsizei GetWidth() const { return width; }
  GLsizei GetHeight() const { return height; }
  GLuint GetChannelCount() const { return nChannels;}
  void Use(GLuint);
  GLuint GetID() const { return ID; }
  void SetTexParameteri(GLuint parameter, int value) const;
};

#endif
