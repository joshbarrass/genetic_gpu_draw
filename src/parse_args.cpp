#include <getopt.h>
#include <filesystem>
#include <iostream>
#include "main_class.h"

const std::string HELP_TEXT_FORMAT = R"(%1$s -- Draw Images Genetically (OpenGL Version!)
Usage:
  %1$s -f <target> -n <num_triangles> -o <output> [-g <per_generation>] [-r <resume>]
  %1$s --help

-f --target          Path to image file to aim for
-o --output          Path to output final image to
-v --svg             Path to output SVG to
-n --triangles       Number of triangles to draw
-g --per-generation  Number of triangles to attempt to draw each generation
-s --seed            Set the seed used to generate the triangles
-r --resume          Path to image to load instead of blank canvas
-h --help            Print this message
)";

int Main::parseArgs(int argc, char **argv) {
  this_program = std::string(argv[0]);

  option longopts[] = {{"triangles", required_argument, NULL, 'n'},
                       {"output", required_argument, NULL, 'o'},
                       {"target", required_argument, NULL, 'f'},
                       {"per-generation", required_argument, NULL, 'g'},
                       {"resume", required_argument, NULL, 'r'},
                       {"seed", required_argument, NULL, 's'},
                       {"help", no_argument, NULL, 'h'},
                       {"svg", required_argument, NULL, 'v'},
                       {0}};

  int opt;
  // drop into new scope because this_file may never be initialised
  // and is not needed beyond this
  {std::filesystem::path this_file;
  while ((opt = getopt_long(argc, argv, "n:o:f:g:r:s:v:h", longopts, 0)) != -1) {
    switch (opt) {
    case 'h':
      this_file = std::filesystem::path(argv[0]);
      std::fprintf(stderr, HELP_TEXT_FORMAT.c_str(),
                   this_file.filename().u8string().c_str());
      return 0;
    case 'n':
      ITERATIONS = atoi(optarg);
      set_iterations = true;
      break;
    case 'o':
      OUT_FILE = std::string(optarg);
      set_out_file = true;
      break;
    case 'f':
      IMAGE_FILE = std::string(optarg);
      set_image_file = true;
      break;
    case 'g':
      IMAGES_PER_GENERATION = atoi(optarg);
      break;
    case 'r':
      RESUME_FILE = std::string(optarg);
      set_resume_file = true;
      break;
    case 's':
      SEED = atoi(optarg);
      set_seed = true;
      break;
    case 'v':
      SVG_FILE = std::string(optarg);
      set_svg_file = true;
      break;
    case ':':
      using namespace std;
      cerr << "Option '" << optopt << "' missing arg" << endl;
      break;
    case '?':
      // using namespace std;
      // cerr << "Unrecognised argument: '" << optopt << "'" << endl;
      // return 1;
      break;
    default:
      abort();
    }
  }
  }

  {
    using namespace std;
    if (!set_image_file) {
      cerr << "Required argument --target not set" << endl;
      return 2;
    }
    if (!set_out_file) {
      cerr << "Required argument --output not set" << endl;
      return 2;
    }
    if (!set_iterations) {
      cerr << "Required argument --triangles not set" << endl;
      return 2;
    }
  }
  return 0;
}
