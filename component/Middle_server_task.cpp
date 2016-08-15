#include <iostream>

#include <boost/bind.hpp>

#include "common/Scheduler.h"
#include "common/handle_error.h"
#include "common/Task_manager.h"

#include "Work_item.h"
#include "Middle_server_task.h"

Middle_server_task *g_pmiddle_server_task;

extern int g_middle_server_port;

Middle_server_task::Middle_server_task()
{
	this->m_pagent_nh = NULL;
	this->m_plistener = NULL;
	this->m_task_type = 0;
	this->m_task_discribe = "middle server task";
}

Middle_server_task::~Middle_server_task()
{

}

int Middle_server_task::initial(void *arg, boost::function<void (int, void *)> result_cb_obj)
{
	this->m_plistener = this->open_listener();
	Sock_addr sa;
	sa.set("0.0.0.0", g_middle_server_port);
	if(this->m_plistener->initial_and_listen(this, g_scheduler, sa, 1000, boost::bind(&Middle_server_task::cmd_listen_cb, this, _1, _2, _3)))
	{
		handle_error("Middle_server_task::initial - initial_and_listen");
		return -1;
	}
	return 0;
}

int Middle_server_task::put_work(Task *ptask, boost::function<int (void *)> work_fn_obj, void *arg, boost::function<void (int, void *)> reply_fn_obj)
{
	if(!this->m_pagent_nh)
	{
		handle_error("Middle_server_task::send_work_item - uninit agent_nh");
		return -1;
	}

	this->m_pagent_nh->write_enable();
	Work_item *pwork_item = new Work_item(ptask, work_fn_obj, arg, reply_fn_obj);
	if(this->m_pagent_nh->send_data((void *)&pwork_item, sizeof(pwork_item)))
	{
		handle_error("Middle_server_task::send_work_item - send_data");
		delete pwork_item;
		return -1;
	}
	return 0;
}

void Middle_server_task::cmd_listen_cb(Listener *plistener, int fd, Sock_addr sa)
{
	this->close_listener(this->m_plistener);
	this->m_plistener = NULL;

	this->m_pagent_nh = this->open_agent_nh();
	if(this->m_pagent_nh->initial(this, g_scheduler, fd, NULL, NULL, boost::bind(&Middle_server_task::cmd_agent_nh_cb, this, _1, _2, _3, _4)))
		handle_fatal_error("Middle_server_task::initial - initial");
	this->m_pagent_nh->read_enable();
	this->m_pagent_nh->write_disable();
}

void Middle_server_task::cmd_agent_nh_cb(Agent_nh *pagent_nh, int type, char *buf, int len)
{
	switch(type)
	{
	case NH_READ_CB:
		do{
			Work_item **work_item_buf = (Work_item **)buf;			//use the work item pointer to communicate
			int req_num = len / sizeof(Work_item *);
			for(int i = 0; i < req_num; i++)
			{
				Work_item *pwork_item = work_item_buf[i];
				pwork_item->reply();								//call back!!!!!
				delete pwork_item;
			}
		}while(0);
		break;

	case NH_WRITE_CB:
		this->m_pagent_nh->write_disable();
		break;

	case NH_CONNECT_CB:
		handle_fatal_error("Middle server connection error");
		break;

	case NH_EOF_CB:
		handle_fatal_error("Middle server connection closed");
		break;

	case NH_ERROR_CB:
		handle_fatal_error("Middle server connection error");
		break;

	case NH_READ_TIMEOUT_CB:
		handle_fatal_error("Middle server connection error");
		break;

	case NH_WRITE_TIMEOUT_CB:
		handle_fatal_error("Middle server connection error");
		break;

	default:
		handle_fatal_error("Middle server connection error");
		break;

	}
}
