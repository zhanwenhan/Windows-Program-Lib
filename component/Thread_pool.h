#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <WinSock2.h>
#include <windows.h>

#include <list>

class Thread_pool
{
public:
	Thread_pool();
	~Thread_pool();

public:
	static DWORD WINAPI work_thread(LPVOID lpParam);

public:
	void initial(int nr_thread, SOCKET sock);
	int run();

private:
	int m_nr_thread;
	SOCKET m_sock;

	std::list<HANDLE> m_thread_list;
};

#endif