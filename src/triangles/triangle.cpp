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
  for (int i = 0; i < 6; i += 2) {
    fCOM[0] += fVertices[i];
    fCOM[1] += fVertices[i+1];
  }
  fCOM[0] /= 3;
  fCOM[1] /= 3;
}

void Triangle::GetArray(float output[TRIANGLE_STRIDE], float alpha) {
  output[0] = fVertices[0];
  output[1] = fVertices[1];
  output[2] = fVertices[2];
  output[3] = alpha;

  output[4] = fVertices[3];
  output[5] = fVertices[4];
  output[6] = fVertices[5];
  output[7] = alpha;

  output[8] = fVertices[6];
  output[9] = fVertices[7];
  output[10] = fVertices[8];
  output[11] = alpha;
}
