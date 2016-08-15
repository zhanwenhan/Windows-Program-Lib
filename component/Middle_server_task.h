#ifndef __MIDDLE_SERVER_TASK_H__
#define __MIDDLE_SERVER_TASK_H__

#include "common/Task.h"
#include "common/Agent_nh.h"
#include "common/Listener.h"

//to manage the communication to thread pool

class Middle_server_task : public Task
{
public:
	Middle_server_task();
	~Middle_server_task();

public:
	int initial(void *arg, boost::function<void (int, void *)> result_cb_obj);
	//add work item to the thread pool
	int put_work(Task *ptask, boost::function<int (void *)> work_fn_obj, void *arg, boost::function<void (int, void *)> reply_fn_obj);

public:
	void cmd_listen_cb(Listener *plistener, int fd, Sock_addr sa);
	void cmd_agent_nh_cb(Agent_nh *pagent_nh, int type, char *buf, int len);

protected:
	Listener *m_plistener;
	Agent_nh *m_pagent_nh;
};

extern Middle_server_task *g_pmiddle_server_task;

#endif