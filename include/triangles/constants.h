#ifndef TRIANGLES_CONSTANTS_H
#define TRIANGLES_CONSTANTS_H 1

// size of each vertex in floats
//   0: x-coord
//   1: y-coord
//   2: z-coord
//   3: COM x-coord
//   4: COM y-coord
constexpr int VERTEX_STRIDE = 5;
constexpr int TRIANGLE_STRIDE = VERTEX_STRIDE * 3;

#endif
