#ifndef __HANDLE_SYS_ERROR_H__
#define __HANDLE_SYS_ERROR_H__

extern void handle_overall_error(int errorno, char *buf);
extern void handle_overall_fatal_error(int errorno, char *buf);

#endif