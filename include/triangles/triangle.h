#ifndef TRIANGLE_CLASS_H
#define TRIANGLE_CLASS_H 1

#include "triangles/constants.h"

class Triangle {
public:
  Triangle(const float[TRIANGLE_STRIDE]);
  Triangle(const float[9]);
  Triangle(const float);
  ~Triangle() {};

  void MoveCOM(const float, const float);
  void GetCOM(float &x, float &y) const {
    x = fCOM[0];
    y = fCOM[1];
  }
  void GetArray(float output[TRIANGLE_STRIDE]) const;
  void GetVertices2D(float output[6]) const;
  
private:
  void calculateCOM();
  
  float fCOM[2];
  float fVertices[9];
};

#endif
