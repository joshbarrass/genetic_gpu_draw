#ifndef TRIANGLE_CLASS_H
#define TRIANGLE_CLASS_H 1

#include "triangles/constants.h"

class Triangle {
public:
  Triangle(const float[9]);
  Triangle(const float);
  ~Triangle() {};

  void MoveCOM(const float, const float);
  void GetCOM(float &x, float &y) {
    x = fCOM[0];
    y = fCOM[1];
  }
  void GetArray(float output[TRIANGLE_STRIDE]);
private:
  void calculateCOM();
  
  float fCOM[2];
  float fVertices[9];
};

#endif
