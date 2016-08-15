#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <windows.h>

#include <iostream>
#include <cstdlib>

#include "common/Scheduler.h"
#include "common/Sock_addr.h"
#include "common/Task_manager.h"
#include "component/Basic_config_manager.h"

#include "Main_body.h"

extern	char	g_localhost_ip[64];
extern	char	g_log_file_path[512];
extern	int		g_persist_timeout;
extern	int		g_middle_server_port;
extern	int		g_thread_pool_nr;

int main()
{
#ifdef WIN32
	WSADATA wsaData;
	DWORD Ret;
	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)	
	{
		std::cout << "WSAStartup failed with error" << std::endl;
		return -1;
	}
#endif

	Basic_config_manager basic_config_manager;
	if(basic_config_manager.initial("./config.xml"))		//get the config xml
	{
		std::cout << "load basic config failed" << std::endl;
		return -1;
	}
	std::cout << "local ip: " << g_localhost_ip << std::endl;
	std::cout << "log file path: " << g_log_file_path << std::endl;
	std::cout << "persist timeout: " << g_persist_timeout << std::endl;
	std::cout << "middle server listen port: " << g_middle_server_port << std::endl;
	std::cout << "thread pool size: " << g_thread_pool_nr << std::endl;

	Main_body main_body;
	main_body.initial();
	while(1)						//main thread block!!!
		Sleep(90000000);

#ifdef WIN32
	WSACleanup();
#endif
	return 0;
}
