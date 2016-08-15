#include <cstdio>
#include <assert.h>

#include <boost/bind.hpp>

#include "common/handle_error.h"
#include "component/Middle_server_task.h"

#include "Log_task.h"

extern	char	g_log_file_path[512];

Log_task::Log_task()
{
	this->m_log_file_hd = NULL;
	this->m_task_type = 2;
	this->m_task_discribe = "log task";
}

Log_task::~Log_task()
{

}

int Log_task::initial(void *arg, boost::function<void (int, void *)> result_cb_obj)
{
	assert(result_cb_obj == NULL);
	this->m_log_file_hd = CreateFile(g_log_file_path, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
	if(this->m_log_file_hd == INVALID_HANDLE_VALUE)
	{
		handle_error("Log_task::initial - CreateFile");
		return -1;
	}
	LARGE_INTEGER position = {0};
	if(!SetFilePointerEx(this->m_log_file_hd, position, NULL, FILE_END))
	{
		handle_error("Log_task::initial - SetFilePointerEx");
		return -1;
	}
	return 0;
}

void Log_task::log_record(std::string &log_str)
{
	SYSTEMTIME now_time;
	GetLocalTime(&now_time);
	char time_buf[64];
	std::sprintf(time_buf, "%02d:%02d:%02d:%03d", now_time.wHour, now_time.wMinute, now_time.wSecond, now_time.wMilliseconds);
	std::string *precord_str = new std::string(time_buf);
	*precord_str += "\t";
	*precord_str += log_str;
	*precord_str += "\r\n";
	dev_log(precord_str->c_str());
	g_pmiddle_server_task->put_work(this, boost::bind(&Log_task::work_fn, this, _1), (void *)precord_str, boost::bind(&Log_task::reply_fn, this, _1, _2));
}

int Log_task::work_fn(void *arg)
{
	std::string *precord_str = (std::string *)arg;
	if(this->write_file_nr(this->m_log_file_hd, precord_str->c_str(), precord_str->length()) == -1)
		return -1;
	return 0;
}

void Log_task::reply_fn(int wrong, void *arg)
{
	delete arg;
	if(wrong)
		handle_error("record log error");
}

int Log_task::write_file_nr(HANDLE hd, const char *str, int size)
{
	DWORD nleft = 0;
	DWORD nwriten = 0;
	const char *ptr = NULL;

	ptr = str;
	nleft = size;
	while(nleft > 0)
	{
		if(!WriteFile(hd, ptr, nleft, &nwriten, NULL))
			return -1;
		nleft -= nwriten;
		ptr += nwriten;
	}

	return size;
}

Log_task *g_plog_task;