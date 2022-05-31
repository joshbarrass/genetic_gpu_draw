#include <iostream>
#include "stb/stb_image.h"

#include "textureClass.h"

Texture::Texture(const char *texturePath, const GLenum srcColorScheme, const GLenum destColorScheme) : haveGivenUnitWarning(false) {
  // load the image data
  stbi_set_flip_vertically_on_load(true);
  int channels = 0;
  switch (srcColorScheme) {
  case GL_RGB:
    channels = 3;
    break;
  case GL_RGBA:
    channels = 4;
    break;
  default:
    break;
  }
  unsigned char *data = stbi_load(texturePath, &width, &height, &nChannels, 0);
  if (!data) {
    std::cerr << "ERROR::TEXTURE::LOADING\nFailed to load texture file '" << texturePath << "'" << std::endl;
  }

  // create a new texture and bind it as the current 2D texture
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);
  
  // set the texture wrapping/filtering options (on currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // generate the texture from the image data we have loaded
  //   1) Load into the currently bound 2D texture
  //   2) Mipmap level (used when manually adding mipmaps)
  //   3) Colour scheme to store as (e.g. RGB)
  //   4/5) Width/Height
  //   6) Always 0, for legacy reasons
  //   7) Colour scheme of source image
  //   8) Datatype the source image is currently stored as
  //   9) Pointer to the actual image data
  glTexImage2D(GL_TEXTURE_2D, 0, destColorScheme, width, height, 0, srcColorScheme, GL_UNSIGNED_BYTE, data);

  // free up the now-unneeded stbi image 
  stbi_image_free(data);
}

void Texture::Use(GLuint unitNumber) {
  // attempt to fix -- maybe GL_TEXTURE was given rather than just an index
  if (unitNumber > 15) {
    if (!haveGivenUnitWarning) {
      std::cerr << "WARNING::TEXTURE::USE::INVALID_UNIT\n" << "Unit number '" << unitNumber << "' lies outside the valid range. Will attempt to correct, but this may cause unexpected behaviour." << std::endl;
      haveGivenUnitWarning = true;
    }
    unitNumber -= GL_TEXTURE0;
  }
  // now assert that it's correct
  // don't test < 0 since this is a uint
  if (unitNumber > 15) {
    std::cerr << "ERROR::TEXTURE::USE::INVALID_UNIT\n" << "Unit numbers must be betweeen 0 and 15, not '" << unitNumber << "'" << std::endl;
  }
  glActiveTexture(GL_TEXTURE0 + unitNumber);
  glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::SetTexParameteri(GLuint parameter, int value) const {
  glBindTexture(GL_TEXTURE_2D, ID);
  glTexParameteri(GL_TEXTURE_2D, parameter, value);
}
