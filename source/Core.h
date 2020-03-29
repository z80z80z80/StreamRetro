#include "LibRetroGameWrapper.h"

#include "libretro.h"
#include "configuration.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <alsa/asoundlib.h>

class Core
{
public:
  //Core()  { };
  Core(const std::string module_path, const std::string game_path)
  : libretro(LibRetroGameWrapper(module_path)), game_path(game_path)
    {
    };

  ~Core()
  {
    if (libretro.initialized == true) {
      libretro.retro_deinit();
      audio_deinit();
      video_deinit();

      glfwTerminate();
    }

  };
  void load();

  void load_game();

  void run();


private:
  LibRetroGameWrapper libretro;
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

  static bool environment(unsigned cmd, void *data);
  static void video_refresh(const void* data, unsigned width, unsigned height, size_t pitch);
  static void input_poll();
  static int16_t input_state(unsigned port, unsigned device, unsigned index, unsigned id);
  static void audio_sample(int16_t left, int16_t right);
  static size_t audio_sample_batch(const int16_t *data, size_t frames);

  static void refresh_vertex_data();
  static void resize_cb(GLFWwindow* win, int w, int h);
  static void create_window(int width, int height);
  static void resize_to_aspect(double ratio, int sw, int sh, int* dw, int* dh);
  static void video_configure(const retro_game_geometry* geom);
  static bool video_set_pixel_format(unsigned format);
  static void video_render();
  static void video_deinit();
  static size_t audio_write(const void *buf, unsigned frames);
  static void audio_init(int frequency);
  static void audio_deinit() { snd_pcm_close(g_pcm); };

  static void log(enum retro_log_level level, const char *fmt, ...);
};
