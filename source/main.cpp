/**
 * author: Orell Garten
 * date: 21/MAR/2020
 * licence: MIT
 *
 * This is the main file showing how to load dynamic libraries
 *
 **/

// include Core for game
#include "StreamRetroGame.h"

// CPP includes
#include <iostream>

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

    using namespace StreamRetro;
    StreamRetroGame game(argv[1], argv[2]);
    game.run();

    return 0;
}
