#include <algorithm>
#include "triangles/constants.h"
#include "triangles/collection.h"
#include "triangles/triangle.h"
#include "rand.h"
#include "consts.h"

#include <iostream>

TriangleCollection::TriangleCollection(const int n) : fNumTriangles(n) {
  fTriangles = new float[n*TRIANGLE_STRIDE];

  // construct OGL structures
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_STRIDE*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTEX_STRIDE*sizeof(float), (void*)(sizeof(float)*3));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);
}

void TriangleCollection::UpdateBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*fNumTriangles*TRIANGLE_STRIDE, &fTriangles[0], GL_DYNAMIC_DRAW);
}

// update only one triangle
void TriangleCollection::UpdateBuffer(const int i) {
  int triangle = i * TRIANGLE_STRIDE;
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, triangle*sizeof(float), sizeof(float)*TRIANGLE_STRIDE, &fTriangles[triangle]);
}

TriangleCollection::~TriangleCollection() { delete[] fTriangles; }

void TriangleCollection::RandomiseAll() {
  float zStep = (triCollectionMaxZ - triCollectionMinZ) / (fNumTriangles-1);
  for (int i = 0; i < fNumTriangles; ++i) {
    int triangle = i * TRIANGLE_STRIDE;
    Triangle t(i * zStep + triCollectionMinZ);
    t.GetArray(&fTriangles[triangle]);
  }
}

void TriangleCollection::Randomise_i(int i) {
  // get the current triangle
  float current_vertices[TRIANGLE_STRIDE];
  GetTriangle(i, current_vertices);

  // extract the current  z position
  // these should be the same for each vertex
  float z = current_vertices[2];

  // generate new triangle
  int triangle = i * TRIANGLE_STRIDE;
  Triangle t(z);
  t.GetArray(&fTriangles[triangle]);
}

void TriangleCollection::Draw() const {
  Draw(fNumTriangles);
}

void TriangleCollection::Draw(const int n) const {
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glDrawArrays(GL_TRIANGLES, 0, 3*n);
}

void TriangleCollection::DrawOne(const int i) const {
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glDrawArrays(GL_TRIANGLES, 3*i, 3);
}

// get a triangle directly from the buffer. This will be
// TRIANGLE_STRIDE floats. For a more friendly function, use
// GetTriangle(int)
void TriangleCollection::GetTriangle(int i, float *vertices_out) const {
  std::copy(fTriangles + i*TRIANGLE_STRIDE, fTriangles + i*TRIANGLE_STRIDE + TRIANGLE_STRIDE, vertices_out);
}

Triangle TriangleCollection::GetTriangle(int i) const {
  // fetch the internal representation of the triangle
  float internal[TRIANGLE_STRIDE];
  GetTriangle(i, internal);

  return Triangle(internal, true);
}

void TriangleCollection::PrintVBOContents() const {
  float data[fNumTriangles * TRIANGLE_STRIDE];
  glBindVertexArray(VAO);
  glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*fNumTriangles*TRIANGLE_STRIDE, data);

  for (int i = 0; i < fNumTriangles * TRIANGLE_STRIDE; ++i) {
    std::cout << data[i] << " ";
  }
  std::cout << std::endl;
}
