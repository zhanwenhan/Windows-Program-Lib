#include <iostream>
#include <map>
#include <string.h>
#include <stdlib.h>

#include "common/handle_error.h"

#include "Basic_config_manager.h"

char	g_localhost_ip[64];
char	g_log_file_path[512];
int		g_persist_timeout = 0;
int		g_middle_server_port = 0;
int		g_thread_pool_nr = 0;

Basic_config_manager::Basic_config_manager()
{

}

Basic_config_manager::~Basic_config_manager()
{

}

int Basic_config_manager::initial(std::string xml_file)
{
	if(!this->m_xml_wrapper.load_from_file(xml_file.c_str()))
	{
		handle_error("Basic_config_manager::initial - m_xml_wrapper.load_from_file");
		return -1;
	}

	if(this->g_localhost_ip_init())
		return -1;
	if(this->g_log_file_path_init())
		return -1;
	if(this->g_persist_timeout_init())
		return -1;
	if(this->g_middle_server_port_init())
		return -1;
	if(this->g_thread_pool_nr_init())
		return -1;

	return 0;
}

void Basic_config_manager::show_config()
{
	std::cout << "local host ip: " << g_localhost_ip << std::endl;
	std::cout << "log file path: " << g_log_file_path << std::endl;
	std::cout << "persist timeout: " << g_persist_timeout << std::endl;
	std::cout << "middle server port: " << g_middle_server_port << std::endl;
	std::cout << "thread pool number: " << g_thread_pool_nr << std::endl;
}

int Basic_config_manager::g_middle_server_port_init()
{
	std::string tmp_str;
	if(this->m_xml_wrapper.get_first_node_value_by_xpath("/CONFIG/BASIC_CONFIG/MIDDLE_SERVER_PORT", tmp_str))
	{
		handle_error("Basic_config_manager::config_init - /CONFIG/BASIC_CONFIG/MIDDLE_SERVER_PORT");
		return -1;
	}

	g_middle_server_port = atoi(tmp_str.c_str());
	if(g_middle_server_port < 0 || g_middle_server_port > 65535)
	{
		handle_error("Basic_config_manager::config_init - /CONFIG/BASIC_CONFIG/MIDDLE_SERVER_PORT");
		return -1;
	}
	return 0;
}

int Basic_config_manager::g_thread_pool_nr_init()
{
	std::string tmp_str;
	if(this->m_xml_wrapper.get_first_node_value_by_xpath("/CONFIG/BASIC_CONFIG/THREAD_POOL_NR", tmp_str))
	{
		handle_error("Basic_config_manager::config_init - /CONFIG/BASIC_CONFIG/THREAD_POOL_NR");
		return -1;
	}

	g_thread_pool_nr = atoi(tmp_str.c_str());
	if(g_thread_pool_nr < 0 || g_thread_pool_nr > 10)
	{
		handle_error("Basic_config_manager::config_init - /CONFIG/BASIC_CONFIG/THREAD_POOL_NR");
		return -1;
	}
	return 0;
}

int Basic_config_manager::g_persist_timeout_init()
{
	std::string tmp_str;
	if(this->m_xml_wrapper.get_first_node_value_by_xpath("/CONFIG/BASIC_CONFIG/PERSIST_TIMEOUT", tmp_str))
	{
		handle_error("Basic_config_manager::config_init - /CONFIG/BASIC_CONFIG/PERSIST_TIMEOUT");
		return -1;
	}

	g_persist_timeout = atoi(tmp_str.c_str());
	if(g_persist_timeout < 0 || g_persist_timeout > 100)
	{
		handle_error("Basic_config_manager::config_init - /CONFIG/BASIC_CONFIG/PERSIST_TIMEOUT");
		return -1;
	}
	return 0;
}

int Basic_config_manager::g_localhost_ip_init()
{
	std::string tmp_str;
	if(this->m_xml_wrapper.get_first_node_value_by_xpath("/CONFIG/BASIC_CONFIG/LOCALHOST_IP", tmp_str))
	{
		handle_error("Basic_config_manager::config_init - /CONFIG/BASIC_CONFIG/LOCALHOST_IP");
		return -1;
	}

	memcpy(g_localhost_ip, tmp_str.c_str(), tmp_str.length());
	return 0;
}

int Basic_config_manager::g_log_file_path_init()
{
	std::string tmp_str;
	if(this->m_xml_wrapper.get_first_node_value_by_xpath("/CONFIG/BASIC_CONFIG/LOG_FILE_PATH", tmp_str))
	{
		handle_error("Basic_config_manager::config_init - /CONFIG/BASIC_CONFIG/LOG_FILE_PATH");
		return -1;
	}

	memcpy(g_log_file_path, tmp_str.c_str(), tmp_str.length());
	return 0;
}
