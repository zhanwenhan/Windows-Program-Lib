#include <Windows.h>

#include <iostream>
#include <string>

#include "handle_sys_error.h"

void handle_overall_error(int errorno, char *buf)
{
	OutputDebugString(buf);
	std::cout << buf << std::endl;
}


void handle_overall_fatal_error(int errorno, char *buf)
{
	OutputDebugString(buf);
	std::cout << buf << std::endl;
	exit(errorno);
}
