#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/event.h>

#include "handle_error.h"
#include "Agent_nh.h"

Agent_nh::Agent_nh()
{
	this->m_is_recycled = 0;
	this->m_bev = NULL;
	this->m_ptask = NULL;
}

Agent_nh::~Agent_nh()
{
	this->recycle();
}

void Agent_nh::recycle()
{
	if(this->m_is_recycled)
		return;
	this->m_is_recycled = 1;
	if(this->m_bev)
	{
		bufferevent_free(this->m_bev);
		this->m_bev = NULL;
	}
	this->m_ptask = NULL;

	return;
}

int Agent_nh::is_recycled()
{
	return this->m_is_recycled;
}



void Agent_nh::read_cb(struct bufferevent *bev, void *arg)
{
	Agent_nh *pthis = (Agent_nh *)arg;
	struct evbuffer *input = bufferevent_get_input(bev);
	while(evbuffer_get_length(input))
	{
		char buf[1024];
		size_t len = bufferevent_read(bev, buf, 1024);
		pthis->m_cb_obj(pthis, NH_READ_CB, buf, len);
	}
}

void Agent_nh::write_cb(struct bufferevent *bev, void *arg)
{
	Agent_nh *pthis = (Agent_nh *)arg;
	struct evbuffer *output = bufferevent_get_output(bev);
	if (evbuffer_get_length(output) == 0) 
		pthis->m_cb_obj(pthis, NH_WRITE_CB, NULL, 0);
}

void Agent_nh::event_cb(struct bufferevent *bev, short what, void *arg)
{
	Agent_nh *pthis = (Agent_nh *)arg;
 	if (what & BEV_EVENT_EOF)
 		pthis->m_cb_obj(pthis, NH_EOF_CB, NULL, 0);
 	else if (what & BEV_EVENT_ERROR)
		pthis->m_cb_obj(pthis, NH_ERROR_CB, NULL, 0);
 	else if ((what & (BEV_EVENT_TIMEOUT | BEV_EVENT_READING)) == (BEV_EVENT_TIMEOUT | BEV_EVENT_READING))
 		pthis->m_cb_obj(pthis, NH_READ_TIMEOUT_CB, NULL, 0);
	else if ((what & (BEV_EVENT_TIMEOUT | BEV_EVENT_WRITING)) == (BEV_EVENT_TIMEOUT | BEV_EVENT_WRITING))
		pthis->m_cb_obj(pthis, NH_WRITE_TIMEOUT_CB, NULL, 0);
 	else if (what & BEV_EVENT_CONNECTED)
		pthis->m_cb_obj(pthis, NH_CONNECT_CB, NULL, 0);
}



int Agent_nh::initial(Task *ptask, Scheduler &sched, evutil_socket_t fd, Time_value *pcrtv, Time_value *pcwtv, 
					  boost::function<void (Agent_nh *, int, char *, int)> cb_obj)
{
	struct timeval temp_rtv, temp_wtv;
	struct timeval *ptemp_rtv = NULL;
	struct timeval *ptemp_wtv = NULL;

	this->m_ptask = ptask;
	this->m_cb_obj = cb_obj;
	this->m_bev = bufferevent_socket_new(sched.get_base(), fd, BEV_OPT_CLOSE_ON_FREE);
	if (!this->m_bev) 
	{
		handle_error("Agent_nh::initial - error constructing bufferevent");
		return -1;
	}
	bufferevent_setcb(this->m_bev, Agent_nh::read_cb, Agent_nh::write_cb, Agent_nh::event_cb, this);

	if(pcrtv)
	{
		temp_rtv = pcrtv->get_timeval();
		ptemp_rtv = &temp_rtv;
	}
	if(pcwtv)
	{
		temp_wtv = pcwtv->get_timeval();
		ptemp_wtv = &temp_wtv;
	}
	bufferevent_set_timeouts(this->m_bev, ptemp_rtv, ptemp_wtv);
	return 0;
}

int Agent_nh::connect(Sock_addr &csa)
{
	struct sockaddr_in sin;
	sin = csa.get_sockaddr_in();
	return bufferevent_socket_connect(this->m_bev, (struct sockaddr*)&sin, sizeof(struct sockaddr_in));
}

int Agent_nh::send_data(const void *data, size_t size)
{
	return bufferevent_write(this->m_bev, data, size);
}

Sock_addr Agent_nh::get_peer_addr()
{
	SOCKET sock = (SOCKET)bufferevent_getfd(this->m_bev);
	sockaddr_in sin;
	std::memset(&sin, 0, sizeof(sin));
	int len = sizeof(sin);
	if(getpeername(sock, (sockaddr *)&sin, &len))
		handle_error("Agent_nh::get_peer_addr - getpeername");
	Sock_addr sa;
	sa.set_by_sockaddr_in(&sin, sizeof(sin));
	return sa;
}

Sock_addr Agent_nh::get_local_addr()
{
	SOCKET sock = (SOCKET)bufferevent_getfd(this->m_bev);
	sockaddr_in sin;
	std::memset(&sin, 0, sizeof(sin));
	int len = sizeof(sin);
	if(getsockname(sock, (sockaddr *)&sin, &len))
		handle_error("Agent_nh::get_local_addr - getsockname");
	Sock_addr sa;
	sa.set_by_sockaddr_in(&sin, sizeof(sin));
	return sa;
}


bool Agent_nh::is_read_enable()
{
	short what = bufferevent_get_enabled(this->m_bev);
	if(what & EV_READ)
		return true;
	else
		return false;
}

bool Agent_nh::is_write_enable()
{
	short what = bufferevent_get_enabled(this->m_bev);
	if(what & EV_WRITE)
		return true;
	else
		return false;
}

int Agent_nh::read_enable()
{
	return bufferevent_enable(this->m_bev, EV_READ);
}

int Agent_nh::read_disable()
{
	return bufferevent_disable(this->m_bev, EV_READ);
}

int Agent_nh::write_enable()
{
	return bufferevent_enable(this->m_bev, EV_WRITE);
}

int Agent_nh::write_disable()
{
	return bufferevent_disable(this->m_bev, EV_WRITE);
}

