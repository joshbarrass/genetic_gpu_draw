#ifndef TRIANGLE_COLLECTION_H
#define TRIANGLE_COLLECTION_H 1

#include <glad/glad.h>
#include "triangles/triangle.h"

class TriangleCollection {
public:
  TriangleCollection(const int n);
  ~TriangleCollection();
  void UpdateBuffer();
  void UpdateBuffer(const int);
  void RandomiseAll();
  void Randomise_i(int);

  void GetTriangle(int, float*) const;
  Triangle GetTriangle(int) const;

  void Draw() const;
  void Draw(const int) const;
  void DrawOne(const int) const;

  void PrintVBOContents() const;

  int GetNumTriangles() const { return fNumTriangles; }

private:
  float *fTriangles;
  int fNumTriangles;

  GLuint VBO;
  GLuint VAO;
};

#endif
