/*
  SVG implementation specific to this use-case.
  I repeat: THIS IS NOT A GENERIC SVG IMPLEMENTATION!

  Triangles are drawn based on the contents of a TriangleCollection.
  Note: There is no need to dump this from OpenGL, as triangles are
        initially specified in an array in CPU memory, which is
        copied into GPU memory when an update is requested.
  
*/

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <stdlib.h>
#include "triangles/triangle.h"
#include "images/svg.h"
#include "textureClass.h"
#include "texturesampler/sampler.h"

const std::string SVG_header("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n<svg preserveAspectRatio=\"none\" ");
const std::string SVG_header_end(" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">");
const std::string SVG_footer("</svg>");

// functions to convert OGL image coordinates to fractional image coordinates
// OGL coordinates have (-1,1) as the top left and (1,-1) as the bottom right
// images have (0,0) as the top left and (1,1) as the bottom right

float round(float x, int places) {
  const double factor = pow(10, places);
  return round(x*factor) / factor;
}

constexpr float ogl_x_to_image_x(float x) {
  return (x + 1)/2;
}

constexpr float ogl_y_to_image_y(float y) {
  return (1-y)/2;
}

void sample_texture(const Image &im, const float target_x, const float target_y, unsigned char &r, unsigned char &g, unsigned char &b) {
  const int w = im.GetWidth();
  const int h = im.GetHeight();
  // COM is currently in OpenGL coords, i.e. [-1, 1] with (0,0) at
  // the centre and the y-axis going upwards
  // must adjust such that y-axis goes downwards, (0,0) is top left
  // and (1,1) is bottom right
  float conv_x = ogl_x_to_image_x(target_x);
  float conv_y = ogl_y_to_image_y(target_y);
  int x = conv_x * w;
  int y = conv_y * h;

  // validate the range of x and y
  // makes this function slower, but safer
  if (x < 0 || x >= w) {
    using namespace std;
    cerr << "Bad x coord: " << x << endl;
    cerr << "Image width: " << w << endl;
    throw out_of_range("write_svg: x coordinate out of expected range");
  }
  if (y < 0 || y >= h) {
    using namespace std;
    cerr << "Bad y coord: " << y << endl;
    cerr << "Image height: "<< h << endl;
    throw out_of_range("write_svg: y coordinated out of expected range");
  }

  // fetch the colour at the COM
  im.GetPixelValue(x, y, r, g, b);
}

void sample_texture(TextureSampler &sampler, const float x, const float y, float &r, float &g, float &b) {
  sampler.SampleTexture(x, y, r, g, b);
}

void write_polygon(std::ofstream &outfile, const float verts[6],
                   const unsigned char r, const unsigned char g,
                   const unsigned char b) {
  outfile << "<polygon points=\"";

  // large buffer
  char buffer[256];
  std::snprintf(buffer, 255, "%.6f,%.6f %.6f,%.6f %.6f,%.6f", verts[0], -verts[1], verts[2], -verts[3], verts[4], -verts[5]);
  outfile << buffer;

  outfile << "\" stroke=\"none\" ";
  outfile << "fill=\"rgb(" << (int)r << "," << (int)g << "," << (int)b << ")\"/>";
}

void write_svg_header(std::ofstream &outfile, const int w, const int h, const bool AA=false) {
  // write the header
  // defined viewbox matches OpenGL coordinates, except with y-axis inverted
  outfile << SVG_header;
  outfile << "width=\"" << w << "\" height=\"" << h << "\" viewBox=\"-1 -1 2 2\"";
  if (!AA) outfile << " shape-rendering=\"crispEdges\"";
  outfile << SVG_header_end;

  // add a rect representing the background
  outfile << "<rect x=\"-1\" y=\"-1\" width=\"2\" height=\"2\" stroke=\"none\" fill=\"black\"/>";
}

void write_svg_footer(std::ofstream &outfile) {
  outfile << SVG_footer;
}

// write_svg: write an SVG based on a triangle collection and a
// reference image
void write_svg(const TriangleCollection &triangles, const Image &im,
               const char *filename, const bool AA) {
  std::ofstream outfile(filename);

  write_svg_header(outfile, im.GetWidth(), im.GetHeight(), AA);

  // add all the triangles in the collection
  for (int i = 0; i < triangles.GetNumTriangles(); ++i) {
    const Triangle tri = triangles.GetTriangle(i);
    // convert the COM to the image coordinates
    float com_x;
    float com_y;
    tri.GetCOM(com_x, com_y);

    // sample texture at COM
    unsigned char r;
    unsigned char g;
    unsigned char b;
    sample_texture(im, com_x, com_y, r, g, b);

    // get the vertices of this triangle
    float v[6];
    tri.GetVertices2D(v);

    // write the triangle
    write_polygon(outfile, v, r, g, b);
  }

  write_svg_footer(outfile);

  outfile.close();
}

// write_svg based on a triangle collection and a loaded texture
void write_svg(const TriangleCollection &triangles, Texture &tex,
               const char *filename, const bool AA) {
  std::ofstream outfile(filename);

  write_svg_header(outfile, tex.GetWidth(), tex.GetHeight(), AA);

  // setup the texture sampler
  TextureSampler sampler(tex);
  
  // add all the triangles in the collection
  for (int i = 0; i < triangles.GetNumTriangles(); ++i) {
    const Triangle tri = triangles.GetTriangle(i);
    // convert the COM to the image coordinates
    float com_x;
    float com_y;
    tri.GetCOM(com_x, com_y);

    // sample texture at COM
    float r;
    float g;
    float b;
    sample_texture(sampler, com_x, com_y, r, g, b);

    // get the vertices of this triangle
    float v[6];
    tri.GetVertices2D(v);

    // write the triangle
    write_polygon(outfile, v, 255*r, 255*g, 255*b);
  }

  write_svg_footer(outfile);

  outfile.close();
}
