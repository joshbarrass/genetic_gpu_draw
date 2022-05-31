#ifndef TRIANGLES_CONSTANTS_H
#define TRIANGLES_CONSTANTS_H 1

// size of each vertex in floats
//   0: x-coord
//   1: y-coord
//   2: z-coord
//   3: alpha
//   4: COM x-coord
//   5: COM y-coord
constexpr int VERTEX_STRIDE = 6;
constexpr int TRIANGLE_STRIDE = VERTEX_STRIDE * 3;

#endif
