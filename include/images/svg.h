// -*- mode: C++; -*-
#ifndef IMAGES_SVG_H
#define IMAGES_SVG_H 1

#include "images/image.h"
#include "triangles/collection.h"
#include "textureClass.h"

void write_svg(const TriangleCollection &, const Image &,
               const char *filename);
void write_svg(const TriangleCollection &, Texture &,
               const char *filename);

#endif
