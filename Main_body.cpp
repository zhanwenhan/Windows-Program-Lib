#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <windows.h>

#include <iostream>
#include <cstdlib>

#include "common/Scheduler.h"
#include "common/Sock_addr.h"
#include "common/Task_manager.h"

#include "component/Persist_timer_task.h"
#include "component/Middle_server_task.h"
#include "component/Thread_pool.h"
#include "component/Log_task.h"

#include "Main_body.h"
#include "handle_sys_error.h"

#include "Echo_listen_task.h"

extern char g_localhost_ip[64];
extern int g_middle_server_port;
extern int g_thread_pool_nr;

Main_body::Main_body()
{
	this->m_event = 0;
}

Main_body::~Main_body()
{

}

DWORD WINAPI Main_body::main_thread(LPVOID lpParam)
{
	Main_body *pthis = (Main_body *)lpParam;

	if(g_scheduler.initial())
		handle_overall_fatal_error(1, "Main_body::main_thread - g_scheduler.initial");

	Persist_timer_task *ppersist_timer_task = g_task_manager.new_task<Persist_timer_task>();
	if(ppersist_timer_task->initial(NULL, NULL))
		handle_overall_fatal_error(1, "Main_body::main_thread - ppersist_timer_task->initial");

	g_pmiddle_server_task = g_task_manager.new_task<Middle_server_task>();
	if(g_pmiddle_server_task->initial(NULL, NULL))
		handle_overall_fatal_error(1, "Main_body::main_thread - pmiddle_server_task->initial");

	g_plog_task = g_task_manager.new_task<Log_task>();
	if(g_plog_task->initial(NULL, NULL))
		handle_overall_fatal_error(1, "Main_body::main_thread - plog_task->initial");

	if(pthis->app_init())
		handle_overall_fatal_error(1, "application init error");

	if(!SetEvent(pthis->m_event))
		handle_overall_fatal_error(1, "Main_body::main_thread - SetEvent");

	g_scheduler.run();
	return 0;
}

int Main_body::app_init()
{
	// TODO:
	Echo_listen_task *pecho_listen_task = g_task_manager.new_task<Echo_listen_task>();
	if(pecho_listen_task->initial(NULL, NULL))
		handle_overall_fatal_error(1, "Main_body::app_init - pecho_listen_task->initial");
	return 0;
}

int Main_body::initial()
{
	this->m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(this->m_event == NULL)
		handle_overall_fatal_error(1, "Main_body::Init - CreateEvent");

	HANDLE hthread = CreateThread(NULL, 0, main_thread, (LPVOID)this, 0, NULL);

	if(WaitForSingleObject(this->m_event, INFINITE) == WAIT_FAILED)
		handle_overall_fatal_error(1, "Main_body::Init - WaitForSingleObject");

	SOCKET middle_server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);			//connect the middle server
	if(middle_server_sock == INVALID_SOCKET)
		handle_overall_fatal_error(1, "Main_body::Init - socket");
	struct sockaddr_in middle_server_addr;
	memset((void *)&middle_server_addr, 0, sizeof(middle_server_addr));
	middle_server_addr.sin_family = AF_INET;
	middle_server_addr.sin_addr.s_addr = inet_addr(g_localhost_ip);
	middle_server_addr.sin_port = htons(g_middle_server_port);
	if(connect(middle_server_sock, (SOCKADDR*)&middle_server_addr, sizeof(middle_server_addr)) == SOCKET_ERROR)
		handle_overall_fatal_error(1, "Main_body::Init - connect");

	Thread_pool thread_pool;														//thread pool init
	thread_pool.initial(g_thread_pool_nr, middle_server_sock);
	thread_pool.run();

	dev_log("Main_body Init OK");
	return 0;
}
