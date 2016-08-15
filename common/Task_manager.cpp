#include <algorithm>

#include "handle_error.h"
#include "Task.h"
#include "Task_manager.h"


Task_Manager g_task_manager;

Task_Manager::Task_Manager()
{
	dev_log("task manager construct");
}

Task_Manager::~Task_Manager()
{
	dev_log("task manager deconstruct");
}

void Task_Manager::recycle(Task *ptr)
{	
	this->m_available_task_list.remove(ptr);
	ptr->recycle();
	dev_log("a task recycled");
	this->m_recycle_task_list.remove(ptr);
	this->m_recycle_task_list.push_back(ptr);
}

void Task_Manager::clear_recycle()
{
	std::list<Task *> temp_task_list;
	std::list<Task *>::iterator p_task_iterator;
	for	(
			p_task_iterator = m_recycle_task_list.begin();
			p_task_iterator != m_recycle_task_list.end();
			p_task_iterator++
		)
	{
		if((*p_task_iterator)->get_nr_req())
			temp_task_list.push_back(*p_task_iterator);
		else
			delete *p_task_iterator;
	}
	this->m_recycle_task_list.clear();
	this->m_recycle_task_list = temp_task_list;
}


void Task_Manager::task_list(std::list<Task_item> &lst)
{
	lst.clear();

	std::list<Task *>::iterator p_task_iterator;
	for	(
			p_task_iterator = m_recycle_task_list.begin();
			p_task_iterator != m_recycle_task_list.end();
			p_task_iterator++
		)
	{
		struct Task_item ti;
		ti.task_index = (int)(*p_task_iterator);
		ti.task_type = (*p_task_iterator)->get_task_type();
		ti.task_discribe = (*p_task_iterator)->get_task_discribe();
		lst.push_back(ti);
	}
}

int Task_Manager::task_kill(int task_index)
{
	Task *ptr = (Task *)task_index;
	std::list<Task *>::const_iterator resault = std::find(this->m_available_task_list.begin(), this->m_available_task_list.end(), ptr);
	if(resault == this->m_available_task_list.end())
		return -1;
	this->recycle(ptr);
	return 0;
}