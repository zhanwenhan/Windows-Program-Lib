#ifndef __HANDLE_ERROR_H__
#define __HANDLE_ERROR_H__


extern void dev_log(const char *str);
extern void handle_error(const char *str);
extern void handle_syscall_error(const char *str);
extern void handle_fatal_error(const char *str);
extern void handle_fatal_syscall_error(const char *str);

#endif
