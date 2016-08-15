#ifndef __BASIC_CONFIG_MANAGER__
#define __BASIC_CONFIG_MANAGER__

#include "component/Xml_Wrapper.h"

class Basic_config_manager
{
public:
	Basic_config_manager();
	~Basic_config_manager();

public:
	int initial(std::string xml_file);
	void show_config();

private:
	int g_localhost_ip_init();
	int g_log_file_path_init();
	int g_persist_timeout_init();
	int g_middle_server_port_init();
	int g_thread_pool_nr_init();

private:
	Xml_Wrapper m_xml_wrapper;
};

#endif