/**
 * author: Orell Garten
 * date: 21/MAR/2020
 * licence: MIT
 *
 * This is the main file showing how to load dynamic libraries
 *
 **/

// include Core for game
#include "Core.h"
#include "LibRetroGameWrapper.h"

#include <dlfcn.h>

// CPP includes
#include <iostream>
#include <memory>
#include <string>

int
main(int argc, char** argv)
{
  if (argc < 3) {
    std::cerr << "usage: " << argv[0] << "  <core> <game>\n";
    return -1;
  }

  if (!glfwInit()) {
    std::cerr << "Failed to initialize glfw\n";
    return -1;
  }


  Core core(argv[1], argv[2]);

  core.load();
  core.load_game();
  core.run();

  return 0;
}
