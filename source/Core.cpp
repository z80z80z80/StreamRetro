/**
 * author: Orell Garten
 * date: 29/MAR/2020
 * license: MIT
 *
 * This file provides the implementation of the core functionality of libretro
 * */

#include "libretro.h"
#include <iostream>

#include "Core.h"

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <zmq.hpp>
#include "zhelpers.hpp"
#include <unistd.h>

// generate zmq socket and connect to server
zmq::context_t context_sub (1);
zmq::context_t context_pub (1);

static zmq::socket_t subscriber (context_sub, ZMQ_SUB);
static std::string message;

static zmq::socket_t publisher (context_pub, ZMQ_PUB);

namespace StreamRetro {

void Core::set_callbacks() {
    libretro.set_environment(environment);
    libretro.set_video_refresh(video_refresh);
    libretro.set_input_poll(input_poll);
    libretro.set_input_state(input_state);
    libretro.set_audio_sample(audio_sample);
    libretro.set_audio_sample_batch(audio_sample_batch);
}

/***
 * This function loads the game and gets the game ready to be played
 * Also initializes the communication between different system parts
 */
void
Core::load_game()
{
    // initialize the socket connection between libretro frontend and http server
    subscriber.connect("tcp://127.0.0.1:1312");
    const char *filter = "";
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));

    // initialize the socket connection between libretro frontend and the websocket relais
    publisher.bind("tcp://*:4242");

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

/***
 * Starts the game.
 */
void
Core::run()
{
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

        // calculate the number of pixels (x3 for RGB)
        int n = width * height * 3;

        // screen buffer, 3 * 1 byte per pixel
        unsigned char* scrBuffer = (unsigned char*)malloc(n * sizeof(char));

        // grabbing the raw pixel data in RGB888
        glReadPixels((GLint)0, (GLint)0,
                     (GLint)width, (GLint)height,
                     GL_RGB, GL_UNSIGNED_BYTE, scrBuffer);


            // using TurboJPEG to write the data correctly
            int jpegQual = 75;
            int flags = 0;
            unsigned char* jpegBuffer = NULL;
            unsigned long jpegSize = 0;

            // compress the raw data to jpeg
            tjhandle  handle = tjInitCompress();
            int tj_stat = tjCompress2(handle, scrBuffer, width, width * 3, height,
                                      TJPF_RGB, &(jpegBuffer), &jpegSize, TJSAMP_411, jpegQual, flags);
 
            /*
            FILE *file = fopen("static/tmp.jpg", "wb");
            fwrite(jpegBuffer, jpegSize, 1, file);

            // close the file
            // image still needs to be flipped (we can do this easier in css)
            fclose(file);
            system("mv static/tmp.jpg static/img.jpg");
            */

            zmq::message_t img_data(jpegSize);
            memcpy((char*) img_data.data(), jpegBuffer, jpegSize);
    
            publisher.send(img_data);
        // free the allocated memory
        free(scrBuffer);

    }
}

void
Core::update_keys(std::string key){
    int i;
    for (i=0; i<10; i++){
        if (not joykeys[i].v){
            joykeys[i].v = (joykeys[i].d == key); // if button is marked as not pressed and is physically pressed -> mark as pressed
        }
        else{
            joykeys[i].v = (joykeys[i].u != key); // if button is marked as pressed but is physically released -> mark as unpressed 
        }
    }

}

bool
Core::get_key(std::string name){
    int i;
    bool value;
    for (i=0; i<10; i++){
        if (joykeys[i].n == name){
            value = joykeys[i].v;
        }
    }
    return value;
}

void
Core::input_poll()
{

    // receive the data in non-blocking mode
    std::string key = "NONE";
    message = s_recv(subscriber, ZMQ_NOBLOCK);

/*
    static unsigned g_joy[RETRO_DEVICE_ID_JOYPAD_R3 + 1] = { 0 };

    static std::vector<keymap> g_binds = { { GLFW_KEY_X, "a_down", RETRO_DEVICE_ID_JOYPAD_A },
                                           { GLFW_KEY_Z, "y_down", RETRO_DEVICE_ID_JOYPAD_B },
                                           { GLFW_KEY_A, "sl_down", RETRO_DEVICE_ID_JOYPAD_Y },
                                           { GLFW_KEY_S, "sr_down", RETRO_DEVICE_ID_JOYPAD_X },
                                           { GLFW_KEY_UP, "up_down", RETRO_DEVICE_ID_JOYPAD_UP },
                                           { GLFW_KEY_DOWN, "down_down", RETRO_DEVICE_ID_JOYPAD_DOWN },
                                           { GLFW_KEY_LEFT, "left_down", RETRO_DEVICE_ID_JOYPAD_LEFT },
                                           { GLFW_KEY_RIGHT, "right_down", RETRO_DEVICE_ID_JOYPAD_RIGHT },
                                           { GLFW_KEY_ENTER, "zr_down", RETRO_DEVICE_ID_JOYPAD_START },
                                           { GLFW_KEY_BACKSPACE, "zl_down", RETRO_DEVICE_ID_JOYPAD_SELECT },

                                           { 0, "", 0 } };
*/


    key = message;
    update_keys(key); 

    int i;
    for (i = 0; g_binds[i].k || g_binds[i].rk; ++i)
        g_joy[g_binds[i].rk] = ((glfwGetKey(g_win, g_binds[i].k) == GLFW_PRESS) || (get_key(g_binds[i].s) == true));

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
    int16_t buf[2] = { left, right };
    audio_write(buf, 1);
}

size_t
Core::audio_sample_batch(const int16_t* data, size_t frames)
{
    return audio_write(data, frames);
}

size_t
Core::audio_write(const void* buf, unsigned frames)
{
    int written = snd_pcm_writei(g_pcm, buf, frames);

    if (written < 0) {
        std::cout << "Alsa warning/error # " << -written << ": ";
        snd_pcm_recover(g_pcm, written, 0);

        return 0;
    }

    return written;
}

void
Core::audio_init(int frequency)
{
    int err;

    if ((err = snd_pcm_open(&g_pcm, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
        std::cerr << "Failed to open playback device: " << std::string(snd_strerror(err));

    err = snd_pcm_set_params(g_pcm, SND_PCM_FORMAT_S16, SND_PCM_ACCESS_RW_INTERLEAVED, 2, frequency, 1, 64 * 1000);

    if (err < 0)
        std::cerr << "Failed to configure playback device: " << std::string(snd_strerror(err));
}

void Core::audio_deinit()
{
    snd_pcm_close(g_pcm);
}

void
Core::video_deinit()
{
    if (g_video.tex_id)
        glDeleteTextures(1, &g_video.tex_id);

    g_video.tex_id = 0;
}

void
Core::video_render()
{
    glBindTexture(GL_TEXTURE_2D, g_video.tex_id);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, g_vertex);
    glTexCoordPointer(2, GL_FLOAT, 0, g_texcoords);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

bool
Core::video_set_pixel_format(unsigned format)
{
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
            g_video.pixfmt = GL_UNSIGNED_SHORT_5_6_5;
            g_video.pixtype = GL_RGB;
            g_video.bpp = sizeof(uint16_t);
            break;
        default:
            std::cerr << "Unknown pixel type " << format << "\n";
    }

    return true;
}

void
Core::video_configure(const retro_game_geometry* geom)
{
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

    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA8, geom->max_width, geom->max_height, 0, g_video.pixtype, g_video.pixfmt, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);

    g_video.tex_w = geom->max_width;
    g_video.tex_h = geom->max_height;
    g_video.clip_w = geom->base_width;
    g_video.clip_h = geom->base_height;

    refresh_vertex_data();
}

void
Core::resize_to_aspect(double ratio, int sw, int sh, int* dw, int* dh)
{
    *dw = sw;
    *dh = sh;

    if (ratio <= 0)
        ratio = (double)sw / sh;

    if ((float)sw / sh < 1)
        *dw = *dh * ratio;
    else
        *dh = *dw / ratio;
}

void
Core::refresh_vertex_data()
{
    assert(g_video.tex_w);
    assert(g_video.tex_h);
    assert(g_video.clip_w);
    assert(g_video.clip_h);

    GLfloat* coords = g_texcoords;
    coords[1] = coords[5] = (float)g_video.clip_h / g_video.tex_h;
    coords[4] = coords[6] = (float)g_video.clip_w / g_video.tex_w;
}

void
Core::resize_cb(GLFWwindow* win, int w, int h)
{
    glViewport(0, 0, w, h);
}

void
Core::create_window(int width, int height)
{
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

void
Core::log(enum retro_log_level level, const char* fmt, ...)
{
    char buffer[4096] = { 0 };
    static const char* levelstr[] = { "dbg", "inf", "wrn", "err" };
    va_list va;

    va_start(va, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, va);
    va_end(va);

    if (level == 0)
        return;

    std::cerr << "[" << levelstr[level] << "] " << buffer << "\n";

    if (level == RETRO_LOG_ERROR)
        exit(EXIT_FAILURE);
}

}
