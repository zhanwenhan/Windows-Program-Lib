#ifndef __AGENT_H__
#define __AGENT_H__

#include <event2/bufferevent.h>
#include <event2/util.h>
#include <boost/function.hpp>

#include "Scheduler.h"
#include "Time_value.h"
#include "Sock_addr.h"

#include "Basic_struct.h"

#define READ_CB				1
#define WRITE_CB			2
#define ERROR_CB			3
#define EOF_CB				4
#define CONNECT_CB			5
#define READ_TIMEOUT_CB		6
#define WRITE_TIMEOUT_CB	7

class Task;

//wrap the socket and the buffer with the header

class Agent
{
public:
	Agent();
	~Agent();

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
		boost::function<void (Agent *, int, Header, char *)> cb_obj);
	//connect the remote peer
	int connect(Sock_addr &csa);
	//send data to peer
	int send_data(const void *data, size_t size);
	//get peer sock addr
	Sock_addr get_peer_addr();
	Sock_addr get_local_addr();

	//read and write control
	bool is_read_enable();
	bool is_write_enable();
	int read_enable();
	int read_disable();
	int write_enable();
	int write_disable();

private:
	struct bufferevent *m_bev;		//pointer to buffer event

	int m_is_head;					//judge the head
	int m_readn;					//read count
	Header m_header;				//protocol head received
	char *m_buf;					//protocol content received

	int m_is_recycled;				//recycle mark

	boost::function<void (Agent *, int, Header, char *)> m_cb_obj;		//function object to callback
	Task *m_ptask;					//owner task
};

#endif