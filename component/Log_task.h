#ifndef __LOG_TASK_H__
#define __LOG_TASK_H__

#include <Windows.h>
#include <string>

#include "common/Task.h"

//to record the log

class Log_task : public Task
{
public:
	Log_task();
	~Log_task();

public:
	int initial(void *arg, boost::function<void (int, void *)> result_cb_obj);
	void log_record(std::string &log_str);

public://work to thread pool
	int work_fn(void *arg);
	void reply_fn(int wrong, void *arg);

protected:
	HANDLE m_log_file_hd;

protected:
	int write_file_nr(HANDLE hd, const char *str, int size);
};

extern Log_task *g_plog_task;

#endif