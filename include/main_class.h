#ifndef MAIN_CLASS_H
#define MAIN_CLASS_H 1

#include <string>
#include <GLFW/glfw3.h>

class Main {
public:
  Main();
  ~Main();
  int parseArgs(int argc, char **argv);
  void register_signal_callback();
  int run();
private:
  void signal_callback(int);
  void handleKBInterrupt();
  void process_input(GLFWwindow *window);

  // args to parse
  std::string IMAGE_FILE;
  bool set_image_file;
  std::string OUT_FILE;
  bool set_out_file;
  int ITERATIONS;
  bool set_iterations;
  int IMAGES_PER_GENERATION;
  std::string RESUME_FILE;
  bool set_resume_file;
  int SEED;
  bool set_seed;
  std::string SVG_FILE;
  bool set_svg_file;

  bool shouldStartRendering;

  // used for outputting resume command
  std::string this_program;

  bool FINISH_NOW;
};

#endif
