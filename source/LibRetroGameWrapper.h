/**
 * author: Orell Garten
 * date: 22/MAR/2020
 * license: MIT
 *
 * This file provides a class which wraps a known function from a shared object
 * */

#ifndef DYNAMIC_FUNCTION_WRAPPER
#define DYNAMIC_FUNCTION_WRAPPER

#include <dlfcn.h>
#include <string>

#include "libretro.h"
#include <iostream>
class LibRetroGameWrapper
{
public:
  LibRetroGameWrapper(const std::string module_name)
    : mod(module_name)
    , handle(dlopen(mod.c_str(), RTLD_LAZY)){};
  LibRetroGameWrapper(const LibRetroGameWrapper& rhs) = default;

  ~LibRetroGameWrapper()
  {
    dlclose(handle);
  };

  // load all symbol
  int load_external_symbols();

  void retro_init()
  {
    ext_retro_init();
  };
  void retro_deinit()
  {
    ext_retro_deinit();
  };
  unsigned retro_api_version()
  {
    return ext_retro_api_version();
  };
  void retro_get_system_info(retro_system_info& info)
  {
    ext_retro_get_system_info(&info);
  };
  void retro_get_system_av_info(retro_system_av_info& info)
  {
    ext_retro_get_system_av_info(&info);
  };
  void retro_set_controller_port_device(unsigned port, unsigned device)
  {
    ext_retro_set_controller_port_device(port, device);
  };
  void retro_reset()
  {
    ext_retro_reset();
  };
  void retro_run()
  {
    ext_retro_run();
  };
  bool retro_load_game(const retro_game_info& game)
  {
    return ext_retro_load_game(&game);
  };
  void retro_unload_game()
  {
    ext_retro_unload_game();
  };

  void set_environment(retro_environment_t env)
  {
    ext_set_environment(env);
  };
  void set_video_refresh(retro_video_refresh_t vr)
  {
    ext_set_video_refresh(vr);
  };
  void set_input_poll(retro_input_poll_t ip)
  {
    ext_set_input_poll(ip);
  };
  void set_input_state(retro_input_state_t is)
  {
    ext_set_input_state(is);
  };
  void set_audio_sample(retro_audio_sample_t as)
  {
    ext_set_audio_sample(as);
  };
  void set_audio_sample_batch(retro_audio_sample_batch_t asb)
  {
    ext_set_audio_sample_batch(asb);
  };

  bool initialized = false;

private:
  std::string mod;
  void* handle;

  // pointers to libretro functions
  void (*ext_retro_init)(void);
  void (*ext_retro_deinit)(void);
  unsigned (*ext_retro_api_version)(void);
  void (*ext_retro_get_system_info)(struct retro_system_info* info);
  void (*ext_retro_get_system_av_info)(struct retro_system_av_info* info);
  void (*ext_retro_set_controller_port_device)(unsigned port, unsigned device);
  void (*ext_retro_reset)(void);
  void (*ext_retro_run)(void);
  bool (*ext_retro_load_game)(const struct retro_game_info* game);
  void (*ext_retro_unload_game)(void);

  void (*ext_set_environment)(retro_environment_t) = NULL;
  void (*ext_set_video_refresh)(retro_video_refresh_t) = NULL;
  void (*ext_set_input_poll)(retro_input_poll_t) = NULL;
  void (*ext_set_input_state)(retro_input_state_t) = NULL;
  void (*ext_set_audio_sample)(retro_audio_sample_t) = NULL;
  void (*ext_set_audio_sample_batch)(retro_audio_sample_batch_t) = NULL;
};
#endif
