#ifndef TRIANGLE_CLASS_H
#define TRIANGLE_CLASS_H 1

class Triangle {
private:
  float fCOM[2];
  float fVertices[9];
public:
  Triangle(const float vertices[9]);
  ~Triangle() {};
};

#endif
