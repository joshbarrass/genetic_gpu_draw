#ifndef TRIANGLE_CLASS_H
#define TRIANGLE_CLASS_H 1

#include "triangles/constants.h"

class Triangle {
public:
  Triangle(const float *vertices, const bool is_internal=false);
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
  void copy_array_as_coords(const float vertices[9]);
  void copy_array_as_internal(const float vertices[TRIANGLE_STRIDE]);
  
  float fCOM[2];
  float fVertices[9];
};

#endif
