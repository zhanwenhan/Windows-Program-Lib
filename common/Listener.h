#ifndef __LISTENER_H__
#define __LISTENER_H__

#include <event2/listener.h>
#include <boost/function.hpp>

#include "Scheduler.h"
#include "Sock_addr.h"

class Task;

class Listener
{
public:
	Listener();
	~Listener();

public:
	void recycle();
	int is_recycled();

public:
	//listener callback
	static void listen_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *arg);

public:
	//initial a listener and begin listen
	int initial_and_listen(Task *ptask, Scheduler &sched, Sock_addr &sa, int backlog, boost::function<void (Listener *, int, Sock_addr)> cb_obj);

private:
	struct evconnlistener *m_listener;					//the listener

	int m_is_recycled;									//recycle mark

	boost::function<void (Listener *, int, Sock_addr)> m_cb_obj;	//callback
	Task *m_ptask;										//owner task
};

#endif