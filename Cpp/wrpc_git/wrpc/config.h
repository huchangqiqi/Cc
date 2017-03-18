#ifndef _CONFIG_H_
#define _CONFIG_H_
#include "config_loader.hpp"
#include <string>
#define MODULE_NAME "rabbitmq"
extern std::string g_rabbitmq_server_addr;

static inline void init_config()
{
	std::string module = std::string(MODULE_NAME);
#ifdef WIN32
	Config_loader config(get_program_path() + "\\" + CONFIG_FILE_PATH, module);
#else
	Config_loader config(get_program_path() + "/" + CONFIG_FILE_PATH, module);
#endif
	g_rabbitmq_server_addr = config.get_config(module, "rabbitmq_server_addr");
}


#endif
