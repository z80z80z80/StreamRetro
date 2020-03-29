#include "LibRetroGameWrapper.h"

#include <iostream>
#include <string>

#include <dlfcn.h>

int
LibRetroGameWrapper::load_external_symbols()
{
  if (!handle) {
    return -1;
    const char* error = dlerror();
    if (error != NULL) {
      std::cerr << error << "\n";
      return -1;
    }
  } // clear errors
  dlerror();

  // pointers to libretro functions
  *(void**)(&ext_retro_init) = dlsym(handle, "retro_init");
  *(void**)(&ext_retro_deinit) = dlsym(handle, "retro_deinit");
  *(void**)(&ext_retro_api_version) = dlsym(handle, "retro_api_version");
  *(void**)(&ext_retro_get_system_info) = dlsym(handle, "retro_get_system_info");
  *(void**)(&ext_retro_get_system_av_info) = dlsym(handle, "retro_get_system_av_info");
  *(void**)(&ext_retro_set_controller_port_device) = dlsym(handle, "retro_set_controller_port_device");
  *(void**)(&ext_retro_reset) = dlsym(handle, "retro_reset");
  *(void**)(&ext_retro_run) = dlsym(handle, "retro_run");
  *(void**)(&ext_retro_load_game) = dlsym(handle, "retro_load_game");
  *(void**)(&ext_retro_unload_game) = dlsym(handle, "retro_undload_game");

  *(void**)(&ext_set_environment) = dlsym(handle, "retro_set_environment");
  *(void**)(&ext_set_video_refresh) = dlsym(handle, "retro_set_video_refresh");
  *(void**)(&ext_set_input_poll) = dlsym(handle, "retro_set_input_poll");
  *(void**)(&ext_set_input_state) = dlsym(handle, "retro_set_input_state");
  *(void**)(&ext_set_audio_sample) = dlsym(handle, "retro_set_audio_sample");
  *(void**)(&ext_set_audio_sample_batch) = dlsym(handle, "retro_set_audio_sample_batch");

  const char* error = dlerror();
  if (error != NULL) {
    std::cerr << error << "\n";
    return -1;
  }

  return 0;
}


