#include <iostream>
#include <string>
#include <filesystem>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "images/image.h"
#include "textureClass.h"
#include "framebuffer.h"
#include "main_class.h"
#include "shaderClass.h"
#include "triangles/constants.h"
#include "triangles/collection.h"
#include "progressBar.h"
#include "rand.h"

constexpr int PROGRESS_BAR_SIZE = 30;

// uint8_t WINDOW_COLOR[4] = {0xdd, 0xcc, 0xff, 0xff};
const double INITIAL_WINDOW_COLOR[4] = {0x00/255., 0x00/255., 0x00/255., 0xff/255.};

Main::Main() : set_image_file(false), set_out_file(false), set_iterations(false), IMAGES_PER_GENERATION(1), set_resume_file(false), set_seed(false), FINISH_NOW(false), shouldStartRendering(false) {}
Main::~Main() {}

// callback to resize the framebuffer if the user resizes the window
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// respond to input
void Main::process_input(GLFWwindow *window) {
  // exit on ESC
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  // start rendering on SPACE
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    shouldStartRendering = true;
  }
}

#define glClearColorArray(color) \
  glClearColor(color[0], color[1], color[2], color[3])

#define glClearColorByteArray(color) \
  glClearColor(color[0] / 255., color[1] / 255., color[2] / 255., color[3] / 255.)

int Main::run() {
  // seed rng
  if (!set_seed) {
    SEED = time(0);
  }
  std::cerr << "Seed: " << SEED << std::endl;
  seed(SEED);
  
  // load the target image to get the width
  int WINDOW_WIDTH;
  int WINDOW_HEIGHT;
  {
    Image im(IMAGE_FILE);
    WINDOW_WIDTH = im.GetWidth();
    WINDOW_HEIGHT = im.GetHeight();
  }

  /* initialise a GLFW window (LearnOpenGL 4) */
  glfwInit();

  // set required OpenGL version and profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  // create the window
  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Genetic GPU Draw", NULL, NULL);
  if (!window) {
    std::cerr << "Failed to create GLFW window!" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  /* initialise GLAD so we can access OpenGL function pointers (LearnOpenGL 4.1) */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialise GLAD!" << std::endl;
    glfwTerminate();
    return -1;
  }

  /* set viewport size */
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  /* register callback */
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // load the image as a texture for use in the shader
  Texture target(IMAGE_FILE.c_str(), GL_RGB, GL_RGB);
  constexpr GLuint targetUnitNumber = 0;

  /* build a collection of triangles */
  TriangleCollection Triangles(ITERATIONS);
  Triangles.RandomiseAll(false);
  Triangles.SetTriangleVisibility(0, true);
  Triangles.UpdateBuffer();

  #ifdef BUILD_DEBUG
  Triangles.PrintVBOContents();
  float t1[TRIANGLE_STRIDE];
  Triangles.GetTriangle(0, t1);
  {
    using namespace std;
    for (int v = 0; v < 3; ++v) {
      cout << "v" << v << ": ";
      for (int i = 0; i < 3; ++i) {
        cout << t1[VERTEX_STRIDE*v + i] << ",";
      }
      cout << " Alpha: " << t1[VERTEX_STRIDE*v + 3];
      cout << ", COM: " << t1[VERTEX_STRIDE*v + 4] << "," << t1[VERTEX_STRIDE*v + 5] << endl;
    }
  }
  float t2[TRIANGLE_STRIDE];
  Triangles.GetTriangle(1, t2);
  {
    using namespace std;
    for (int v = 0; v < 3; ++v) {
      cout << "v" << v << ": ";
      for (int i = 0; i < 3; ++i) {
        cout << t2[VERTEX_STRIDE*v + i] << ",";
      }
      cout << " Alpha: " << t2[VERTEX_STRIDE*v + 3];
      cout << ", COM: " << t2[VERTEX_STRIDE*v + 4] << "," << t2[VERTEX_STRIDE*v + 5] << endl;
    }
  }
  #endif

  // build the shader program
  Shader simpleShader("./shaders/triangle_vert_shader.glsl", "./shaders/simpleFragShader.glsl");

  glClearColorArray(INITIAL_WINDOW_COLOR);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable( GL_BLEND );

  // glDisable(GL_DEPTH_TEST);
  // glDisable(GL_CULL_FACE);
  // glDisable(GL_ALPHA_TEST);
  
  // wait in a basic loop
  // while (!glfwWindowShouldClose(window) && !shouldStartRendering) {
  //   process_input(window);
  //   glClear(GL_COLOR_BUFFER_BIT);
  //   glfwSwapBuffers(window);
  //   glfwPollEvents();
  // }
  // if (glfwWindowShouldClose(window)) {
  //   return 0;
  // }

  simpleShader.use();

  FramebufferTexture canvas(WINDOW_WIDTH, WINDOW_HEIGHT);
  canvas.Use();

  ProgressBar pbar(PROGRESS_BAR_SIZE, 0, ITERATIONS, true);
  int i = pbar.GetValue();
  while (i < ITERATIONS && !glfwWindowShouldClose(window)) {
    // check for premature exit
    if (FINISH_NOW) {
      // exit via the built-in method; admittedly it shouldn't make a
      // difference
      glfwSetWindowShouldClose(window, true);
      continue;
    }
    glClear(GL_COLOR_BUFFER_BIT);

    // enable the next triangle
    Triangles.SetTriangleVisibility(i, true);
    Triangles.UpdateBuffer();

    // draw the collection
    simpleShader.use();
    target.Use(targetUnitNumber);
    simpleShader.setInt("target_image", targetUnitNumber);
    Triangles.Draw();

    // glFlush();

    glfwPollEvents();
    ++pbar;
    pbar.Display();
    i = pbar.GetValue();
  }
  std::cerr << std::endl;
  // glfwSwapBuffers(window);

  std::cerr << "Waiting for rendering to complete..." << std::endl;
  glFinish();
  std::cerr << "Copying canvas to CPU..." << std::endl;
  Image fboutput(canvas);
  std::cerr << "Saving..." << std::endl;
  fboutput.Save(OUT_FILE);

  glfwTerminate();
  return 0;
}

int main(int argc, char **argv) {
  Main program = Main();;
  if (int err = program.parseArgs(argc, argv); err != 0) {
    return err;
  }
  program.register_signal_callback();
  return program.run();
}
