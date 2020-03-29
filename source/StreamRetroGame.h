/**
 * author: Orell Garten
 * date: 29/MAR/2020
 * license: MIT
 *
 * This file provides the Game interface
 * */

#ifndef STREAMRETRO_STREAMRETROGAME_H
#define STREAMRETRO_STREAMRETROGAME_H

#include "Core.h"
#include "LibRetroGameWrapper.h"

#include <string>

namespace StreamRetro {
class StreamRetroGame
{
  public:
    /***
     * This constructs a StreamRetroGame for a given libretro module and a specific game
     *
     * @param module_path libretro module
     * @param game_path path to the game
     */
    StreamRetroGame(const std::string module_path, const std::string game_path)
      : libretro(LibRetroGameWrapper(module_path))
      , core(libretro, game_path)
    {
        // load libretro symbols from the module
        int res = libretro.load_external_symbols();

        // set core callbacks to libretro callbacks
        core.set_callbacks();

        // if no errors reported
        if (res == 0) {
            libretro.retro_init();
            libretro.initialized = true;
        }
        std::cout << "setup libretro \n";
        // setup game
        core.load_game();

        std::cout << "game core loaded \n";
    };

    ~StreamRetroGame()
    {
        if (libretro.initialized == true) {
            libretro.retro_deinit();
            glfwTerminate();
        }
    };

    void run()
    {
        core.run();
    };

  private:
    LibRetroGameWrapper libretro;
    Core core;
};
}
#endif // STREAMRETRO_STREAMRETROGAME_H
