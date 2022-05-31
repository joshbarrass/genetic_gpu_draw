#include <algorithm>
#include <iostream>

#include "triangles/triangle.h"

// constexpr int perCoord = 2;
// constexpr int arrayLength = 3*perCoord;

Triangle::Triangle(const float vertices[6]) {
  // copy vertices to internal
  std::copy(&vertices[0], &vertices[1]+1, &fVertices[0]);
  std::copy(&vertices[2], &vertices[3]+1, &fVertices[3]);
  std::copy(&vertices[4], &vertices[5]+1, &fVertices[6]);
  // set z to 0
  fVertices[2] = 0.0;
  fVertices[5] = 0.0;
  fVertices[8] = 0.0;

  // find COM of triangle
  fCOM[0] = 0.0;
  fCOM[1] = 0.0;
  for (int i = 0; i < 6; i += 2) {
    fCOM[0] += vertices[i];
    fCOM[1] += vertices[i+1];
  }
  fCOM[0] /= 3;
  fCOM[1] /= 3;

  // adjust vertices such that triangle is central
  // x coords
  fVertices[0] -= fCOM[0];
  fVertices[3] -= fCOM[0];
  fVertices[6] -= fCOM[0];
  // y coords
  fVertices[1] -= fCOM[1];
  fVertices[4] -= fCOM[1];
  fVertices[6] -= fCOM[1];
}
