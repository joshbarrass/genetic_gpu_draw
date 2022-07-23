// -*- mode: C++; -*-
#ifndef IMAGES_SVG_H
#define IMAGES_SVG_H 1

#include "images/image.h"
#include "triangles/collection.h"

void write_svg(const TriangleCollection &, const Image &,
               const char *path);

#endif
