#include <algorithm>
#include <iostream>

#include "triangles/constants.h"
#include "triangles/triangle.h"
#include "rand.h"

// constexpr int perCoord = 2;
// constexpr int arrayLength = 3*perCoord;

Triangle::Triangle(const float vertices[9]) {
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

void Triangle::GetArray(float output[TRIANGLE_STRIDE]) {
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
