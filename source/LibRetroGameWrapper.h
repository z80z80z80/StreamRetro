/**
 * author: Orell Garten
 * date: 22/MAR/2020
 * license: MIT
 *
 * This file provides a class which wraps a known function from a shared object
 * */

#ifndef DYNAMIC_FUNCTION_WRAPPER
#define DYNAMIC_FUNCTION_WRAPPER

#include <string>
#include <dlfcn.h>

#include "libretro.h"

class LibRetroGameWrapper {
public:
    LibRetroGameWrapper(const std::string &module_name) : mod(module_name), handle(dlopen(mod.c_str(), RTLD_LAZY)) {
        load_external_symbols();
    };
    ~LibRetroGameWrapper() { dlclose(handle); };
    
    void retro_init() { ext_retro_init(); };
    void retro_deinit() { ext_retro_deinit(); } ;
    unsigned retro_api_version() { return ext_retro_api_version(); };
    void retro_get_system_info(retro_system_info &info) { ext_retro_get_system_info(&info); };
    void retro_get_system_av_info(retro_system_av_info &info) {ext_retro_get_system_av_info(&info); } ;
    void retro_set_controller_port_device(unsigned port, unsigned device) {ext_retro_set_controller_port_device(port, device); };
    void retro_reset() {ext_retro_reset(); };
    void retro_run() {ext_retro_run(); };
    bool retro_load_game(const retro_game_info &game) {return ext_retro_load_game(&game); };
    void retro_unload_game() { ext_retro_unload_game(); };
    

private:
    const std::string &mod;
    void* handle;

    // load a single symbol
    int load_external_symbols();
    // pointers to libretro functions
	void (*ext_retro_init)(void);
	void (*ext_retro_deinit)(void);
	unsigned (*ext_retro_api_version)(void);
	void (*ext_retro_get_system_info)(struct retro_system_info *info);
	void (*ext_retro_get_system_av_info)(struct retro_system_av_info *info);
	void (*ext_retro_set_controller_port_device)(unsigned port, unsigned device);
	void (*ext_retro_reset)(void);
	void (*ext_retro_run)(void);
	bool (*ext_retro_load_game)(const struct retro_game_info *game);
	void (*ext_retro_unload_game)(void);
};
#endif
