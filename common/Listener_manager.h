#ifndef __LISTENER_MANAGER_H__
#define __LISTENER_MANAGER_H__

#include <list>

#include "handle_error.h"
#include "Listener.h"

class Listener_Manager
{
public:
	Listener_Manager();
	~Listener_Manager();

public:
	//create a listener
	Listener *new_listener();
	//recycle a listener
	void recycle(Listener *ptr);
	//delete all listener recycled
	void clear_recycle();

public:
	std::list<Listener *>	m_available_listener_list;

private:
	std::list<Listener *>	m_recycle_listener_list;
};

extern Listener_Manager g_listener_manager;

#endif