#ifndef __AGENT_NH_H__
#define __AGENT_NH_H__

#include <event2/bufferevent.h>
#include <event2/util.h>
#include <boost/function.hpp>

#include "Scheduler.h"
#include "Time_value.h"
#include "Sock_addr.h"

#define NH_READ_CB				1
#define NH_WRITE_CB				2
#define NH_ERROR_CB				3
#define NH_EOF_CB				4
#define NH_CONNECT_CB			5
#define NH_READ_TIMEOUT_CB		6
#define NH_WRITE_TIMEOUT_CB		7

class Task;

//wrap the socket and the buffer without the header

class Agent_nh
{
public:
	Agent_nh();
	~Agent_nh();

public:
	//to delete
	void recycle();
	int is_recycled();

public:
	//read callback
	static void read_cb(struct bufferevent *bev, void *arg);
	//write callback
	static void write_cb(struct bufferevent *bev, void *arg);
	//event callback
	static void event_cb(struct bufferevent *bev, short what, void *arg);

public://operation
	//initial: must give task pointer, scheduler, handle, peer sock addr, read time out, write time out, and bind a fn obj to callback
	int initial(Task *ptask, Scheduler &sched, evutil_socket_t fd, Time_value *pcrtv, Time_value *pcwtv, 
		boost::function<void (Agent_nh *, int, char *, int)> cb_obj);
	//connect the remote peer
	int connect(Sock_addr &csa);
	//send data to peer
	int send_data(const void *data, size_t size);
	Sock_addr get_peer_addr();
	Sock_addr get_local_addr();

	//get peer sock addr
	bool is_read_enable();
	bool is_write_enable();
	int read_enable();
	int read_disable();
	int write_enable();
	int write_disable();

private:
	struct bufferevent *m_bev;		//pointer to buffer event

	int m_is_recycled;				//recycle mark

	boost::function<void (Agent_nh *, int, char *, int)> m_cb_obj;		//function object to callback
	Task *m_ptask;					//owner task
};

#endif