//
// Created by orell on 3/29/20.
//

#ifndef STREAMRETRO_CONFIGURATION_H
#define STREAMRETRO_CONFIGURATION_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <alsa/asoundlib.h>

static GLFWwindow* g_win = NULL;
static snd_pcm_t* g_pcm = NULL;
static float g_scale = 3;

static GLfloat g_vertex[] = {
  -1.0f, -1.0f, // left-bottom
  -1.0f, 1.0f,  // left-top
  1.0f,  -1.0f, // right-bottom
  1.0f,  1.0f,  // right-top
};

static GLfloat g_texcoords[] = {
  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
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

struct keymap
{
  unsigned k;
  unsigned rk;
};

static struct keymap g_binds[] = { { GLFW_KEY_X, RETRO_DEVICE_ID_JOYPAD_A },
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

static unsigned g_joy[RETRO_DEVICE_ID_JOYPAD_R3 + 1] = { 0 };



#endif // STREAMRETRO_CONFIGURATION_H
