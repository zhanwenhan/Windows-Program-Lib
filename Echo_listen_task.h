#ifndef __ECHO_LISTEN_TASK_H__
#define __ECHO_LISTEN_TASK_H__


#include "common/Task.h"
#include "common/Listener.h"

class Echo_listen_task : public Task
{
public:
	int initial(void *arg, boost::function<void (int, void *)> result_cb_obj);

public:
	void echo_listen_cb(Listener *plistener, int fd, Sock_addr sa);

protected:
	Listener *m_plistener;
};

#endif