#ifndef __MAIN_BODY_H__
#define __MAIN_BODY_H__

#include <windows.h>

class Main_body
{
public:
	Main_body();
	virtual ~Main_body();

public:
	static DWORD WINAPI main_thread(LPVOID lpParam);

public:
	int initial();
	int app_init();

protected:
	HANDLE m_event;
};

#endif