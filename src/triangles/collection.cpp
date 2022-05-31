#include <algorithm>
#include "triangles/collection.h"
#include "rand.h"

#include <iostream>

// number of values per triangle
// currently 12:
//   0, 4, 8: x-coord
//   1, 5, 9: y-coord
//   2, 6, 10: z-coord
//   3, 7, 11: enable? (bool)
constexpr int VERTEX_STRIDE = 4;
constexpr int TRIANGLE_STRIDE = VERTEX_STRIDE * 3;

TriangleCollection::TriangleCollection(const int n) : fNumTriangles(n) {
  fTriangles = new float[n*TRIANGLE_STRIDE];

  // construct OGL structures
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_STRIDE*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, VERTEX_STRIDE*sizeof(float), (void*)(sizeof(float)*3));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);
}

void TriangleCollection::UpdateBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*fNumTriangles*TRIANGLE_STRIDE, &fTriangles[0], GL_STATIC_DRAW);
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

  int triangle = i * TRIANGLE_STRIDE;
  for (int v = 0; v < 3; ++v) {
    int triangleVertex = triangle + VERTEX_STRIDE*v;
    fTriangles[triangleVertex + 3] = alpha;
  }
}

void TriangleCollection::RandomiseAll(bool visible) {
  // TODO: These values aren't utilised properly
  constexpr float minZ = (-0.5);
  constexpr float maxZ = 0.5;
  float zStep = (maxZ - minZ) / fNumTriangles;

  float alpha;
  if (visible) {
    alpha = 1.0;
  } else {
    alpha = 0.0;
  }

  for (int i = 0; i < fNumTriangles; ++i) {
    int triangle = i * TRIANGLE_STRIDE;
    for (int v = 0; v < 3; ++v) {
      int triangleVertex = triangle + VERTEX_STRIDE * v;
      for (int mu = 0; mu < 2; ++mu) {
        fTriangles[triangleVertex + mu] = randFloat(-1, 1);
      }
      fTriangles[triangleVertex + 2] = i * zStep + minZ;
      fTriangles[triangleVertex + 3] = alpha;
    }
  }
}

void TriangleCollection::Draw() {
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glDrawArrays(GL_TRIANGLES, 0, 3*fNumTriangles);
}

void TriangleCollection::GetTriangle(int i, float *vertices_out) {
  std::copy(fTriangles + i*TRIANGLE_STRIDE, fTriangles + i*TRIANGLE_STRIDE + TRIANGLE_STRIDE, vertices_out);
}

void TriangleCollection::PrintVBOContents() {
  float data[fNumTriangles * TRIANGLE_STRIDE];
  glBindVertexArray(VAO);
  glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*fNumTriangles*TRIANGLE_STRIDE, data);

  for (int i = 0; i < fNumTriangles * TRIANGLE_STRIDE; ++i) {
    std::cout << data[i] << " ";
  }
  std::cout << std::endl;
}
