#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__

#include <list>

#include "handle_error.h"

class Task;

struct Task_item 
{
	int task_index;

	int task_type;
	std::string task_discribe;
};

//factory of task

class Task_Manager
{
public:
	Task_Manager();
	~Task_Manager();

public:
	//create a task
	template<typename Type> Type *new_task()
	{
		Task *ptr = new Type();
		this->m_available_task_list.remove(ptr);
		this->m_available_task_list.push_back(ptr);
		return (Type *)ptr;
	}
	//recycle the task
	void recycle(Task *ptr);
	//clear all task recycled
	void clear_recycle();
	//list all active task
	void task_list(std::list<Task_item> &lst);
	//kill a task by force
	int task_kill(int task_index);

public:
	std::list<Task *>	m_available_task_list;

private:
	std::list<Task *>	m_recycle_task_list;
};

extern Task_Manager g_task_manager;

#endif