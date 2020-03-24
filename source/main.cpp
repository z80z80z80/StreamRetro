/**
 * author: Orell Garten
 * date: 21/MAR/2020
 * licence: MIT
 *
 * This is the main file showing how to load dynamic libraries
 *
 **/

#include <iostream>
#include <string>
#include <memory>

// include Wrapper
#include "LibRetroGameWrapper.h"
#include "libretro.h"

int main(int argc, char* argv[]) {

    // using the wrapper class
    auto wrapper = LibRetroGameWrapper(argv[1]);

   
    return 0;
}
