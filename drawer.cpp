#include <iostream>
#include <string>
#include <filesystem>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "images/image.h"
#include "main_class.h"
#include "shaderClass.h"
#include "triangle.h"
#include "progressBar.h"

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
  // load the target image
  Image target(IMAGE_FILE);
  int WINDOW_WIDTH = target.GetWidth();
  int WINDOW_HEIGHT = target.GetHeight();
  
  /* initialise a GLFW window (LearnOpenGL 4) */
  glfwInit();

  // set required OpenGL version and profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

  /* construct vertices for a scalene triangle -- this has no
     rotational symmetry*/
  // float vertices[6] =
  //   {
  //     -0.5f, -0.33333333f,
  //     0.75f, -0.33333333f,
  //     -0.25f, 0.66666667f
  //   };
  // construct Triangle object
  // Triangle scalene(vertices);

  // build the shader program
  // Shader simpleShader("./shaders/simpleVertShader.glsl", "./shaders/simpleFragShader.glsl");

  glClearColorArray(INITIAL_WINDOW_COLOR);
  
  // wait in a basic loop
  while (!glfwWindowShouldClose(window) && !shouldStartRendering) {
    process_input(window);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  if (glfwWindowShouldClose(window)) {
    return 0;
  }

  ProgressBar pbar(PROGRESS_BAR_SIZE, 0, ITERATIONS, true);
  while (pbar.GetValue() < ITERATIONS && !glfwWindowShouldClose(window)) {
    // check for premature exit
    if (FINISH_NOW) {
      // exit via the built-in method; admittedly it shouldn't make a
      // difference
      glfwSetWindowShouldClose(window, true);
      continue;
    }

    // generate the displacements
    // float xDisp;
    // float yDisp;
    // scalene.GenerateDisplacements(xDisp, yDisp);

    glClear(GL_COLOR_BUFFER_BIT);

    // use our shader to draw our vertices as a triangle
    // simpleShader.use();
    // simpleShader.set3Vec("triColor", brightness, brightness, brightness);
    // simpleShader.setFloat("theta", angle * M_PI / 180.);
    // simpleShader.setFloat("xDisp", xDisp);
    // simpleShader.setFloat("yDisp", yDisp);
    // scalene.Draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
    ++pbar;
    pbar.Display();
  }
  std::cerr << std::endl;

  Image output(window);
  output.Save(OUT_FILE);

  glfwTerminate();
  return 0;
}

int main(int argc, char **argv) {
  Main program = Main();;
  if (int err = program.parseArgs(argc, argv); err != 0) {
    return err;
  }
  program.run();
}
