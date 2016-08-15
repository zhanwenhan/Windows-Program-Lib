#include <iostream>

#include <boost/bind.hpp>

#include "common/handle_error.h"
#include "common/Scheduler.h"
#include "common/Time_value.h"
#include "common/Task_manager.h"

#include "Echo_listen_task.h"
#include "Echo_task.h"

int Echo_listen_task::initial(void *arg, boost::function<void (int, void *)> result_cb_obj)
{
	this->m_plistener = this->open_listener();
	Sock_addr sa;
	sa.set("0.0.0.0", 63211);
	if(this->m_plistener->initial_and_listen(this, g_scheduler, sa, 1000, boost::bind(&Echo_listen_task::echo_listen_cb, this, _1, _2, _3)))
	{
		handle_error("Echo_listen_task::initial - initial_and_listen");
		return -1;
	}
	
	return 0;
}

void Echo_listen_task::echo_listen_cb(Listener *plistener, int fd, Sock_addr sa)
{
	Task *ptask = g_task_manager.new_task<Echo_task>();
	ptask->initial((void *)fd, NULL);
}
