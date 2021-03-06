#include <algorithm>
#include <iostream>

#include "triangles/constants.h"
#include "triangles/triangle.h"
#include "rand.h"

// constexpr int perCoord = 2;
// constexpr int arrayLength = 3*perCoord;

// Initialise a triangle from an array.
// The format of this array varies based on the value of is_internal
//
// is_internal = false (default, for backward compatibility) dictates
// that the vertices array contains 9 floats, corresponding to three
// 3D vectors representing the coordinates in 3D space of the
// triangle's vertices.
//
// is_internal = true dictates that the vertices array contains
// TRIANGLE_STRIDE floats, interpreted as the internal representation
// of a triangle as outputted by GetArray.
Triangle::Triangle(const float *vertices, const bool is_internal) {
  if (is_internal) {
    copy_array_as_internal(vertices);
    return;
  }
  copy_array_as_coords(vertices);
}

// Construct a triangle from the internal format, as outputted by
// GetArray. This method shouild always be kept up-to-date with the
// internal structure of the triangles
void Triangle::copy_array_as_internal(const float vertices[TRIANGLE_STRIDE]) {
  // copy the three vertices
  std::copy(&vertices[0], &vertices[0] + 3, &fVertices[0]);
  std::copy(&vertices[0] + VERTEX_STRIDE, &vertices[0] + VERTEX_STRIDE + 3, &fVertices[3]);
  std::copy(&vertices[0] + VERTEX_STRIDE*2, &vertices[0] + VERTEX_STRIDE*2 + 3, &fVertices[6]);

  // copy the COM
  fCOM[0] = vertices[3];
  fCOM[1] = vertices[4];
}

void Triangle::copy_array_as_coords(const float vertices[9]) {
  // copy vertices to internal
  std::copy(&vertices[0], &vertices[0] + 9, &fVertices[0]);

  calculateCOM();
}

Triangle::Triangle(const float z) {
  // generate random vertices
  fVertices[0] = randFloat(-1, 1);
  fVertices[1] = randFloat(-1, 1);
  fVertices[2] = z;

  fVertices[3] = randFloat(-1, 1);
  fVertices[4] = randFloat(-1, 1);
  fVertices[5] = z;

  fVertices[6] = randFloat(-1, 1);
  fVertices[7] = randFloat(-1, 1);
  fVertices[8] = z;

  calculateCOM();
}

void Triangle::MoveCOM(const float x, const float y) {
  // adjust vertices such that triangle is central
  // x coords
  fVertices[0] += x - fCOM[0];
  fVertices[3] += x - fCOM[0];
  fVertices[6] += x - fCOM[0];
  // y coords
  fVertices[1] += y - fCOM[1];
  fVertices[4] += y - fCOM[1];
  fVertices[6] += y - fCOM[1];

  fCOM[0] = x;
  fCOM[1] = y;
}

void Triangle::calculateCOM() {
  // find COM of triangle (ignoring Z)
  fCOM[0] = 0.0;
  fCOM[1] = 0.0;
  for (int i = 0; i < 9; i += 3) {
    fCOM[0] += fVertices[i];
    fCOM[1] += fVertices[i+1];
  }
  fCOM[0] /= 3.0;
  fCOM[1] /= 3.0;
}

void Triangle::GetArray(float output[TRIANGLE_STRIDE]) const {
  int i = 0;
  output[i+0] = fVertices[0];
  output[i+1] = fVertices[1];
  output[i+2] = fVertices[2];
  output[i+3] = fCOM[0];
  output[i+4] = fCOM[1];

  i = i+VERTEX_STRIDE;
  output[i+0] = fVertices[3];
  output[i+1] = fVertices[4];
  output[i+2] = fVertices[5];
  output[i+3] = fCOM[0];
  output[i+4] = fCOM[1];

  i = i+VERTEX_STRIDE;
  output[i+0] = fVertices[6];
  output[i+1] = fVertices[7];
  output[i+2] = fVertices[8];
  output[i+3] = fCOM[0];
  output[i+4] = fCOM[1];
}

void Triangle::GetVertices2D(float output[6]) const {
  output[0] = fVertices[0];
  output[1] = fVertices[1];
  output[2] = fVertices[3];
  output[3] = fVertices[4];
  output[4] = fVertices[6];
  output[5] = fVertices[7];
}
