#include "common/handle_error.h"
#include "Thread_pool.h"
#include "Work_item.h"

Thread_pool::Thread_pool()
{
	this->m_nr_thread = 0;
	this->m_sock = 0;
}

Thread_pool::~Thread_pool()
{

}

void Thread_pool::initial(int nr_thread, SOCKET sock)
{
	this->m_nr_thread = nr_thread;
	this->m_sock = sock;
}

int Thread_pool::run()
{
	if(this->m_nr_thread == 0 || this->m_sock == 0)
		return -1;
	for(int i = 0; i < this->m_nr_thread; i++)
	{
		HANDLE hthread = CreateThread(NULL, 0, Thread_pool::work_thread, (LPVOID)this->m_sock, 0, NULL);
		if(hthread == NULL)
		{
			handle_error("Thread_pool::run - CreateThread");
			return -1;
		}
		this->m_thread_list.push_back(hthread);
	}
	return 0;
}

DWORD WINAPI Thread_pool::work_thread(LPVOID lpParam)
{
	SOCKET sock = (SOCKET)lpParam;
	
	while(1)
	{
		int ret = 0;
		Work_item *pwork_item = NULL;
		ret = recv(sock, (char *)&pwork_item, sizeof(pwork_item), 0);
		if(ret <= 0)
			handle_fatal_error("Thread_pool::work_thread - recv");
		pwork_item->process();												//do work!!!!
		ret = send(sock, (char *)&pwork_item, sizeof(pwork_item), 0);
		if(ret != sizeof(pwork_item))
			handle_fatal_error("Thread_pool::work_thread - send");
	}
	
	return 0;
}