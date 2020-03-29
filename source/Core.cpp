#include "libretro.h"
#include <iostream>

#include "Core.h"
#include "configuration.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>

void
Core::load()
{
  int res = libretro.load_external_symbols();

  libretro.set_environment(environment);
  libretro.set_video_refresh(video_refresh);
  libretro.set_input_poll(input_poll);
  libretro.set_input_state(input_state);
  libretro.set_audio_sample(audio_sample);
  libretro.set_audio_sample_batch(audio_sample_batch);

  // no error reported
  if (res == 0) {
    libretro.retro_init();
    libretro.initialized = true;
  }
  std::cout << "core loaded \n";
}

void
Core::load_game()
{
  retro_system_av_info av = { 0 };
  retro_system_info system = { 0 };
  retro_game_info info = { game_path.c_str(), 0 };
  FILE* file = fopen(game_path.c_str(), "rb");

  if (!file)
    std::cerr << "Failed to load content '" << game_path << "' : " << std::string(strerror(errno));

  fseek(file, 0, SEEK_END);
  info.size = ftell(file);
  rewind(file);

  libretro.retro_get_system_info(system);

  if (!system.need_fullpath) {
    info.data = malloc(info.size);

    if (!info.data || !fread((void*)info.data, info.size, 1, file))
      std::cerr << "Failed to load content '" << game_path << "' : " << std::string(strerror(errno));
  }

  if (!libretro.retro_load_game(info))
    std::cerr << "The core failed to load the content.\n";

  libretro.retro_get_system_av_info(av);

  video_configure(&av.geometry);
  audio_init(av.timing.sample_rate);
}

void Core::run() {
  while (!glfwWindowShouldClose(g_win)) {
    glfwPollEvents();

    libretro.retro_run();

    glClear(GL_COLOR_BUFFER_BIT);

    video_render();

    glfwSwapBuffers(g_win);
  }
}


/*** PRIVATE MEMBERS ***/

bool
Core::environment(unsigned cmd, void* data)
{
  bool* bval;

  switch (cmd) {
    case RETRO_ENVIRONMENT_GET_LOG_INTERFACE: {
      struct retro_log_callback* cb = (struct retro_log_callback*)data;
      cb->log = log;
      break;
    }
    case RETRO_ENVIRONMENT_GET_CAN_DUPE:
      bval = (bool*)data;
      *bval = true;
      break;
    case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT: {
      const enum retro_pixel_format* fmt = (enum retro_pixel_format*)data;

      if (*fmt > RETRO_PIXEL_FORMAT_RGB565)
        return false;

      return video_set_pixel_format(*fmt);
    }
    case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
    case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
      *(const char**)data = ".";
      return true;

    default:
      log(RETRO_LOG_DEBUG, "Unhandled env #%u", cmd);
      return false;
  }

  return true;
}

void
Core::video_refresh(const void* data, unsigned width, unsigned height, size_t pitch)
{
  if (data) {
    if (g_video.clip_w != width || g_video.clip_h != height) {
      g_video.clip_h = height;
      g_video.clip_w = width;

      refresh_vertex_data();
    }

    glBindTexture(GL_TEXTURE_2D, g_video.tex_id);

    if (pitch != g_video.pitch) {
      g_video.pitch = pitch;
      glPixelStorei(GL_UNPACK_ROW_LENGTH, g_video.pitch / g_video.bpp);
    }

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, g_video.pixtype, g_video.pixfmt, data);
  }
}


void
Core::input_poll()
{
  int i;
  for (i = 0; g_binds[i].k || g_binds[i].rk; ++i)
    g_joy[g_binds[i].rk] = (glfwGetKey(g_win, g_binds[i].k) == GLFW_PRESS);

  // Quit nanoarch when pressing the Escape key.
  if (glfwGetKey(g_win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(g_win, true);
  }
}

int16_t
Core::input_state(unsigned port, unsigned device, unsigned index, unsigned id)
{
  if (port || index || device != RETRO_DEVICE_JOYPAD)
    return 0;

  return g_joy[id];
}

void
Core::audio_sample(int16_t left, int16_t right)
{
  int16_t buf[2] = {left, right};
  audio_write(buf, 1);
}

size_t
Core::audio_sample_batch(const int16_t* data, size_t frames)
{
  return audio_write(data, frames);
}


size_t Core::audio_write(const void *buf, unsigned frames) {
  int written = snd_pcm_writei(g_pcm, buf, frames);

  if (written < 0) {
    std::cout << "Alsa warning/error # " <<  -written << ": ";
    snd_pcm_recover(g_pcm, written, 0);

    return 0;
  }

  return written;
}

void Core::audio_init(int frequency)
{
  int err;

  if ((err = snd_pcm_open(&g_pcm, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
    std::cerr << "Failed to open playback device: " << std::string(snd_strerror(err));

  err = snd_pcm_set_params(g_pcm, SND_PCM_FORMAT_S16, SND_PCM_ACCESS_RW_INTERLEAVED, 2, frequency, 1, 64 * 1000);

  if (err < 0)
    std::cerr << "Failed to configure playback device: " << std::string(snd_strerror(err));
}
void Core::video_deinit() {
  if (g_video.tex_id)
    glDeleteTextures(1, &g_video.tex_id);

  g_video.tex_id = 0;
}

void Core::video_render() {
  glBindTexture(GL_TEXTURE_2D, g_video.tex_id);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(2, GL_FLOAT, 0, g_vertex);
  glTexCoordPointer(2, GL_FLOAT, 0, g_texcoords);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

bool Core::video_set_pixel_format(unsigned format) {
  if (g_video.tex_id)
    std::cerr << "Tried to change pixel format after initialization.";

  switch (format) {
    case RETRO_PIXEL_FORMAT_0RGB1555:
      g_video.pixfmt = GL_UNSIGNED_SHORT_5_5_5_1;
      g_video.pixtype = GL_BGRA;
      g_video.bpp = sizeof(uint16_t);
      break;
    case RETRO_PIXEL_FORMAT_XRGB8888:
      g_video.pixfmt = GL_UNSIGNED_INT_8_8_8_8_REV;
      g_video.pixtype = GL_BGRA;
      g_video.bpp = sizeof(uint32_t);
      break;
    case RETRO_PIXEL_FORMAT_RGB565:
      g_video.pixfmt  = GL_UNSIGNED_SHORT_5_6_5;
      g_video.pixtype = GL_RGB;
      g_video.bpp = sizeof(uint16_t);
      break;
    default:
      std::cerr << "Unknown pixel type " << format << "\n";
  }

  return true;
}

void Core::video_configure(const retro_game_geometry *geom) {
  int nwidth, nheight;

  resize_to_aspect(geom->aspect_ratio, geom->base_width * 1, geom->base_height * 1, &nwidth, &nheight);

  nwidth *= g_scale;
  nheight *= g_scale;

  if (!g_win)
    create_window(nwidth, nheight);

  if (g_video.tex_id)
    glDeleteTextures(1, &g_video.tex_id);

  g_video.tex_id = 0;

  if (!g_video.pixfmt)
    g_video.pixfmt = GL_UNSIGNED_SHORT_5_5_5_1;

  glfwSetWindowSize(g_win, nwidth, nheight);

  glGenTextures(1, &g_video.tex_id);

  if (!g_video.tex_id)
    std::cerr << "Failed to create the video texture\n";

  g_video.pitch = geom->base_width * g_video.bpp;

  glBindTexture(GL_TEXTURE_2D, g_video.tex_id);

//	glPixelStorei(GL_UNPACK_ALIGNMENT, s_video.pixfmt == GL_UNSIGNED_INT_8_8_8_8_REV ? 4 : 2);
//	glPixelStorei(GL_UNPACK_ROW_LENGTH, s_video.pitch / s_video.bpp);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, geom->max_width, geom->max_height, 0,
               g_video.pixtype, g_video.pixfmt, NULL);

  glBindTexture(GL_TEXTURE_2D, 0);

  g_video.tex_w = geom->max_width;
  g_video.tex_h = geom->max_height;
  g_video.clip_w = geom->base_width;
  g_video.clip_h = geom->base_height;

  refresh_vertex_data();
}

void Core::resize_to_aspect(double ratio, int sw, int sh, int *dw, int *dh) {
  *dw = sw;
  *dh = sh;

  if (ratio <= 0)
    ratio = (double)sw / sh;

  if ((float)sw / sh < 1)
    *dw = *dh * ratio;
  else
    *dh = *dw / ratio;
}

void Core::refresh_vertex_data() {
  assert(g_video.tex_w);
  assert(g_video.tex_h);
  assert(g_video.clip_w);
  assert(g_video.clip_h);

  GLfloat *coords = g_texcoords;
  coords[1] = coords[5] = (float)g_video.clip_h / g_video.tex_h;
  coords[4] = coords[6] = (float)g_video.clip_w / g_video.tex_w;
}


void Core::resize_cb(GLFWwindow *win, int w, int h) {
  glViewport(0, 0, w, h);
}


void Core::create_window(int width, int height) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  g_win = glfwCreateWindow(width, height, "nanoarch", NULL, NULL);

  if (!g_win)
    std::cerr << "Failed to create window. \n";

  glfwSetFramebufferSizeCallback(g_win, resize_cb);

  glfwMakeContextCurrent(g_win);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    std::cerr << "Failed to initialize glew\n";

  glfwSwapInterval(1);

  std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

  glEnable(GL_TEXTURE_2D);

//	refresh_vertex_data();

  resize_cb(g_win, width, height);
}

void Core::log(enum retro_log_level level, const char *fmt, ...) {
  char buffer[4096] = {0};
  static const char * levelstr[] = { "dbg", "inf", "wrn", "err" };
  va_list va;

  va_start(va, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, va);
  va_end(va);

  if (level == 0)
    return;

  std::cerr << "["<< levelstr[level] << "] " << buffer << "\n";

  if (level == RETRO_LOG_ERROR)
    exit(EXIT_FAILURE);
}