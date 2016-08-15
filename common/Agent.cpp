#include <WinSock2.h>
#include <Windows.h>

#include <string.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/event.h>

#include "handle_error.h"
#include "Agent.h"

Agent::Agent()
{
	this->m_is_recycled = 0;
	this->m_bev = NULL;
	this->m_ptask = NULL;
	this->m_is_head = 1;
	this->m_readn = 0;
	this->m_buf = NULL;
}

Agent::~Agent()
{
	this->recycle();
}

void Agent::recycle()
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

int Agent::is_recycled()
{
	return this->m_is_recycled;
}



void Agent::read_cb(struct bufferevent *bev, void *arg)
{
	Agent *pthis = (Agent *)arg;
	struct evbuffer *input = bufferevent_get_input(bev);
	while(evbuffer_get_length(input))
	{
		if(pthis->m_is_head)
		{
			size_t len = bufferevent_read(bev, (char *)(&pthis->m_header) + pthis->m_readn, sizeof(Header) - pthis->m_readn);
			pthis->m_readn += len;
			if(pthis->m_readn == sizeof(Header))
			{
				pthis->m_readn = 0;
				if(pthis->m_header.length > 50000000)
				{
					pthis->m_cb_obj(pthis, ERROR_CB, pthis->m_header, "");
					return;
				}
				if(pthis->m_header.length)
				{
					pthis->m_buf = new char[pthis->m_header.length + 1];
					pthis->m_is_head = 0;
				}
				else
					pthis->m_cb_obj(pthis, READ_CB, pthis->m_header, "");
			}
		}
		if(!pthis->m_is_head)
		{
			size_t len = bufferevent_read(bev, pthis->m_buf + pthis->m_readn, pthis->m_header.length - pthis->m_readn);
			pthis->m_readn += len;
			if(pthis->m_readn == pthis->m_header.length)
			{
				pthis->m_buf[pthis->m_header.length] = NULL;
				pthis->m_cb_obj(pthis, READ_CB, pthis->m_header, pthis->m_buf);
				pthis->m_readn = 0;
				pthis->m_is_head = 1;
				delete [] pthis->m_buf;
				pthis->m_buf = NULL;
			}
		}
	}
}

void Agent::write_cb(struct bufferevent *bev, void *arg)
{
	Agent *pthis = (Agent *)arg;
	struct evbuffer *output = bufferevent_get_output(bev);
	if (evbuffer_get_length(output) == 0) 
		pthis->m_cb_obj(pthis, WRITE_CB, pthis->m_header, "");
}

void Agent::event_cb(struct bufferevent *bev, short what, void *arg)
{
	Agent *pthis = (Agent *)arg;
 	if (what & BEV_EVENT_EOF)
 		pthis->m_cb_obj(pthis, EOF_CB, pthis->m_header, "");
 	else if (what & BEV_EVENT_ERROR)
		pthis->m_cb_obj(pthis, ERROR_CB, pthis->m_header, "");
 	else if ((what & (BEV_EVENT_TIMEOUT | BEV_EVENT_READING)) == (BEV_EVENT_TIMEOUT | BEV_EVENT_READING))
 		pthis->m_cb_obj(pthis, READ_TIMEOUT_CB, pthis->m_header, "");
	else if ((what & (BEV_EVENT_TIMEOUT | BEV_EVENT_WRITING)) == (BEV_EVENT_TIMEOUT | BEV_EVENT_WRITING))
		pthis->m_cb_obj(pthis, WRITE_TIMEOUT_CB, pthis->m_header, "");
 	else if (what & BEV_EVENT_CONNECTED)
		pthis->m_cb_obj(pthis, CONNECT_CB, pthis->m_header, "");

 	return;
}



int Agent::initial(Task *ptask, Scheduler &sched, evutil_socket_t fd, Time_value *pcrtv, Time_value *pcwtv, 
				   boost::function<void (Agent *, int, Header, char *)> cb_obj)
{
	struct timeval temp_rtv, temp_wtv;
	struct timeval *ptemp_rtv = NULL;
	struct timeval *ptemp_wtv = NULL;

	this->m_ptask = ptask;
	this->m_cb_obj = cb_obj;
	this->m_bev = bufferevent_socket_new(sched.get_base(), fd, BEV_OPT_CLOSE_ON_FREE);
	if (!this->m_bev) 
	{
		handle_error("Agent::initial - error constructing bufferevent");
		return -1;
	}
	bufferevent_setcb(this->m_bev, Agent::read_cb, Agent::write_cb, Agent::event_cb, this);

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

int Agent::connect(Sock_addr &csa)
{
	struct sockaddr_in sin;
	sin = csa.get_sockaddr_in();
	return bufferevent_socket_connect(this->m_bev, (struct sockaddr*)&sin, sizeof(struct sockaddr_in));
}

int Agent::send_data(const void *data, size_t size)
{
	return bufferevent_write(this->m_bev, data, size);
}

Sock_addr Agent::get_peer_addr()
{
	SOCKET sock = (SOCKET)bufferevent_getfd(this->m_bev);
	sockaddr_in sin;
	std::memset(&sin, 0, sizeof(sin));
	int len = sizeof(sin);
	if(getpeername(sock, (sockaddr *)&sin, &len))
		handle_error("Agent::get_peer_addr - getpeername");
	Sock_addr sa;
	sa.set_by_sockaddr_in(&sin, sizeof(sin));
	return sa;
}

Sock_addr Agent::get_local_addr()
{
	SOCKET sock = (SOCKET)bufferevent_getfd(this->m_bev);
	sockaddr_in sin;
	std::memset(&sin, 0, sizeof(sin));
	int len = sizeof(sin);
	if(getsockname(sock, (sockaddr *)&sin, &len))
		handle_error("Agent::get_local_addr - getsockname");
	Sock_addr sa;
	sa.set_by_sockaddr_in(&sin, sizeof(sin));
	return sa;
}

bool Agent::is_read_enable()
{
	short what = bufferevent_get_enabled(this->m_bev);
	if(what & EV_READ)
		return true;
	else
		return false;
}

bool Agent::is_write_enable()
{
	short what = bufferevent_get_enabled(this->m_bev);
	if(what & EV_WRITE)
		return true;
	else
		return false;
}

int Agent::read_enable()
{
	return bufferevent_enable(this->m_bev, EV_READ);
}

int Agent::read_disable()
{
	return bufferevent_disable(this->m_bev, EV_READ);
}

int Agent::write_enable()
{
	return bufferevent_enable(this->m_bev, EV_WRITE);
}

int Agent::write_disable()
{
	return bufferevent_disable(this->m_bev, EV_WRITE);
}

