#ifndef TRIANGLE_COLLECTION_H
#define TRIANGLE_COLLECTION_H 1

#include <glad/glad.h>

class TriangleCollection {
public:
  TriangleCollection(const int n);
  ~TriangleCollection();
  void UpdateBuffer();
  void UpdateBuffer(const int);
  void RandomiseAll();
  void Randomise_i(int);

  void GetTriangle(int, float*);

  void Draw();
  void Draw(const int);

  void PrintVBOContents();
private:
  float *fTriangles;
  int fNumTriangles;

  GLuint VBO;
  GLuint VAO;
};

#endif
