/**
 * author: Orell Garten
 * date: 29/MAR/2020
 * license: MIT
 *
 * This file provides the core functionality of libretro
 *
 * Currently, part of this code is based on https://github.com/heuripedes/nanoarch/
 * */

#ifndef STREAMRETRO_CORE_H
#define STREAMRETRO_CORE_H

#include "LibRetroGameWrapper.h"

#include "libretro.h"

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <alsa/asoundlib.h>

#include <turbojpeg.h>

namespace StreamRetro {

static GLFWwindow* g_win = nullptr;
static snd_pcm_t* g_pcm = nullptr;
static float g_scale = 1;

static GLfloat g_vertex[] = {
    -1.0f, -1.0f, // left-bottom
    -1.0f, 1.0f,  // left-top
    1.0f,  -1.0f, // right-bottom
    1.0f,  1.0f,  // right-top
};

static GLfloat g_texcoords[] = {
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
};

struct keymap
{
    unsigned k;
    unsigned rk;
};

static struct
{
    GLuint tex_id;
    GLuint pitch;
    GLint tex_w, tex_h;
    GLuint clip_w, clip_h;

    GLuint pixfmt;
    GLuint pixtype;
    GLuint bpp;
} g_video = { 0 };

static unsigned g_joy[RETRO_DEVICE_ID_JOYPAD_R3 + 1] = { 0 };

static std::vector<keymap> g_binds = { { GLFW_KEY_X, RETRO_DEVICE_ID_JOYPAD_A },
                                       { GLFW_KEY_Z, RETRO_DEVICE_ID_JOYPAD_B },
                                       { GLFW_KEY_A, RETRO_DEVICE_ID_JOYPAD_Y },
                                       { GLFW_KEY_S, RETRO_DEVICE_ID_JOYPAD_X },
                                       { GLFW_KEY_UP, RETRO_DEVICE_ID_JOYPAD_UP },
                                       { GLFW_KEY_DOWN, RETRO_DEVICE_ID_JOYPAD_DOWN },
                                       { GLFW_KEY_LEFT, RETRO_DEVICE_ID_JOYPAD_LEFT },
                                       { GLFW_KEY_RIGHT, RETRO_DEVICE_ID_JOYPAD_RIGHT },
                                       { GLFW_KEY_ENTER, RETRO_DEVICE_ID_JOYPAD_START },
                                       { GLFW_KEY_BACKSPACE, RETRO_DEVICE_ID_JOYPAD_SELECT },

                                       { 0, 0 } };

class Core
{
  public:
    // Core()  { };
    /***
     * Constructs a game core for a given libretro shared object and game in game_path.
     *
     * @param libretro reference to libretro-instance
     * @param game_path path to game you want to load
     */
    Core(LibRetroGameWrapper& libretro, const std::string game_path)
      : libretro(libretro)
      , game_path(game_path){};

    ~Core()
    {
        audio_deinit();
        video_deinit();
    }


    void set_callbacks();
    void load_game();
    void run();

    LibRetroGameWrapper& libretro;
    std::string game_path;

    /*
     * All of these functions need to be static, otherwise they cannot be set as callback functions in libretro.
     * The first 6 functions can only call oher static functions..
     *
     * TODO: We probably should figure out how to do this more elegantly..
     *
     * ALSO: Code ist now a ugly mix of C and C++ and needs to be completely ported.
     * Code is based on https://github.com/heuripedes/nanoarch/
     */

    static bool environment(unsigned cmd, void* data);
    static void video_refresh(const void* data, unsigned width, unsigned height, size_t pitch);
    static void input_poll();
    static int16_t input_state(unsigned port, unsigned device, unsigned index, unsigned id);
    static void audio_sample(int16_t left, int16_t right);
    static size_t audio_sample_batch(const int16_t* data, size_t frames);

  private:
    static void refresh_vertex_data();
    static void resize_cb(GLFWwindow* win, int w, int h);
    static void create_window(int width, int height);
    static void resize_to_aspect(double ratio, int sw, int sh, int* dw, int* dh);
    static void video_configure(const retro_game_geometry* geom);
    static bool video_set_pixel_format(unsigned format);
    static void video_render();
    static void video_deinit();
    static size_t audio_write(const void* buf, unsigned frames);
    static void audio_init(int frequency);
    static void audio_deinit();

    static void log(enum retro_log_level level, const char* fmt, ...);
};

}

#endif // STREAMRETRO_CORE_H
