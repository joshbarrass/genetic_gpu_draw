#include "images/image.h"
#include <GLFW/glfw3.h>
#include <stb/stb_image_write.h>

// TODO: other channel counts?
void Image::save_image(const char *filename) const {
  stbi_flip_vertically_on_write(FlipVerticallyOnWrite);
  stbi_write_png(filename, fWidth, fHeight, 3, fImageData, 3*fWidth);
}

void Image::Save(const char *filename) const {
  save_image(filename);
}

void Image::Save(const std::string filename) const {
  save_image(filename.c_str());
}

void Image::Save(const std::filesystem::path filename) const {
  save_image(filename.u8string().c_str());
}
