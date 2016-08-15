#include <iostream>
#include <string>
#include <boost/bind.hpp>

#include "common/Scheduler.h"
#include "common/handle_error.h"
#include "common/Task_manager.h"
#include "component/Log_task.h"

#include "Echo_task.h"

int Echo_task::initial(void *arg, boost::function<void (int, void *)> result_cb_obj)
{
	intptr_t fd = (int)arg;
	this->m_pagent_nh = this->open_agent_nh();
	if(this->m_pagent_nh->initial(this, g_scheduler, fd, NULL, NULL, boost::bind(&Echo_task::echo_agent_nh_cb, this, _1, _2, _3, _4)))
	{
		handle_error("Echo_task::initial - initial");
		return -1;
	}
	this->m_pagent_nh->read_enable();
	return 0;
}

void Echo_task::echo_agent_nh_cb(Agent_nh *pagent_nh, int type, char *buf, int len)
{
 	switch(type)
 	{
 	case READ_CB:
		this->do_echo(buf, len);
 		break;

	case WRITE_CB:
		break;

	case EOF_CB:
		std::cout << "closed by peer" << std::endl;
		this->close_agent_nh(this->m_pagent_nh);
		g_task_manager.recycle(this);
		break;

	case ERROR_CB:
		std::cout << "an error" << std::endl;
		this->close_agent_nh(this->m_pagent_nh);
		g_task_manager.recycle(this);
		break;

	case READ_TIMEOUT_CB:
		std::cout << "read timeout" << std::endl;
		this->close_agent_nh(this->m_pagent_nh);
		g_task_manager.recycle(this);
		break;

	case WRITE_TIMEOUT_CB:
		std::cout << "write timeout" << std::endl;
		this->close_agent_nh(this->m_pagent_nh);
		g_task_manager.recycle(this);
		break;

	default:
		std::cout << "undefined event" << std::endl;
		this->close_agent_nh(this->m_pagent_nh);
		g_task_manager.recycle(this);
		break;

 	}
}

void Echo_task::do_echo(char *buf, int len)
{
	this->m_pagent_nh->send_data((void *)buf, len);
	//g_plog_task->log_record(std::string(buf, len));
}