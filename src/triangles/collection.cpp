#include "triangles/collection.h"
#include "rand.h"

// number of values per triangle
// currently 10:
//   0, 3, 6: x-coord
//   1, 4, 7: y-coord
//   2, 5, 8: z-coord
//   9: enable? (bool)
constexpr int TRIANGLE_STRIDE = 10;

TriangleCollection::TriangleCollection(const int n) : fNumTriangles(n) {
  fTriangles = new float[n*TRIANGLE_STRIDE];

  // construct OGL structures
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, TRIANGLE_STRIDE*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, TRIANGLE_STRIDE*sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
}

void TriangleCollection::UpdateBuffer() {
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(fTriangles), fTriangles, GL_DYNAMIC_DRAW);
}

TriangleCollection::~TriangleCollection() { delete[] fTriangles; }

void TriangleCollection::RandomiseAll() {
  RandomiseAll(false);
}

void TriangleCollection::SetTriangleVisibility(int i, bool visible) {
  float alpha;
  if (visible) {
    alpha = 1.0;
  } else {
    alpha = 0.0;
  }
  fTriangles[i * TRIANGLE_STRIDE + 9] = alpha;
}

void TriangleCollection::RandomiseAll(bool visible) {
  constexpr float minZ = (-0.99);
  constexpr float maxZ = 0.99;
  float zStep = (maxZ - minZ) / fNumTriangles; 
  for (int i = 0; i < fNumTriangles; ++i) {
    int triangle = i * TRIANGLE_STRIDE;
    for (int v = 0; v < 3; ++v) {
      int triangleVertex = triangle + 3 * v;
      for (int mu = 0; mu < 2; ++mu) {
        fTriangles[triangleVertex + mu] = randFloat(-1, 1);
      }
      fTriangles[triangleVertex + 3] = i * zStep + minZ;
    }

    SetTriangleVisibility(i, visible);
  }
}
