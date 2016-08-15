#include "Work_item.h"

Work_item::Work_item(Task *ptask, boost::function<int (void *)> work_fn_obj, void *arg, boost::function<void (int, void *)> reply_fn_obj)
{
	this->m_ptask = ptask;
	this->m_work_fn_obj = work_fn_obj;
	this->m_arg = arg;
	this->m_success = 0;
	this->m_reply_fn_obj = reply_fn_obj;
	this->m_ptask->inc_nr_req();		//inc the ref count
}

Work_item::~Work_item()
{
	this->m_ptask->dec_nr_req();		//dec the ref count
}

void Work_item::process()
{
	this->m_success = this->m_work_fn_obj(this->m_arg);
}

void Work_item::reply()
{
	this->m_reply_fn_obj(this->m_success, this->m_arg);
}