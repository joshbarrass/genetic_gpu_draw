#ifndef TRIANGLES_CONSTANTS_H
#define TRIANGLES_CONSTANTS_H 1

// number of values per triangle
// currently 12:
//   0, 4, 8: x-coord
//   1, 5, 9: y-coord
//   2, 6, 10: z-coord
//   3, 7, 11: enable? (bool)
constexpr int VERTEX_STRIDE = 4;
constexpr int TRIANGLE_STRIDE = VERTEX_STRIDE * 3;

#endif
