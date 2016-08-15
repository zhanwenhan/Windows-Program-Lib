#ifndef __ECHO_TASK_H__
#define __ECHO_TASK_H__

#include "common/Task.h"
#include "common/Agent_nh.h"

class Echo_task : public Task
{
public:
	int initial(void *arg, boost::function<void (int, void *)> result_cb_obj);

public:
	void echo_agent_nh_cb(Agent_nh *pagent_nh, int type, char *buf, int len);

protected:
	Agent_nh *m_pagent_nh;

protected:
	void do_echo(char *buf, int len);
};

#endif