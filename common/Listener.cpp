#include "handle_error.h"
#include "Listener.h"

Listener::Listener()
{
	this->m_is_recycled = 0;
	this->m_listener = NULL;
	this->m_ptask = NULL;
}

Listener::~Listener()
{
	this->recycle();
}



void Listener::recycle()
{
	if(this->m_is_recycled)
		return;
	this->m_is_recycled = 1;
	if(this->m_listener)
	{
		evconnlistener_free(this->m_listener);
		this->m_listener = NULL;
	}
	this->m_ptask = NULL;

	return;
}

int Listener::is_recycled()
{
	return this->m_is_recycled;
}



void Listener::listen_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *arg)
{
	Listener *pthis = (Listener *)arg;
	Sock_addr temp_sa;
	temp_sa.set_by_sockaddr_in((struct sockaddr_in *)sa, socklen);
	pthis->m_cb_obj(pthis, fd, temp_sa);
}



int Listener::initial_and_listen(Task *ptask, Scheduler &sched, Sock_addr &sa, int backlog, boost::function<void (Listener *, int, Sock_addr)> cb_obj)
{
	this->m_ptask = ptask;
	this->m_cb_obj = cb_obj;
	struct sockaddr_in sin;
	sin = sa.get_sockaddr_in();
	this->m_listener = evconnlistener_new_bind(sched.get_base(), Listener::listen_cb, this, 
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, backlog, (struct sockaddr*)&sin, sizeof(struct sockaddr_in));
	if(!this->m_listener)
	{
		handle_error("Listener::initial_and_listen - evconnlistener_new_bind");
		return -1;
	}
	return 0;
}