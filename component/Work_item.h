#ifndef __WORK_ITEM_H__
#define __WORK_ITEM_H__

#include <boost/function.hpp>

#include "common/Task.h"

//wrap the work to the thread pool

class Work_item
{
public:
	explicit Work_item(Task *ptask, boost::function<int (void *)> work_fn_obj, void *arg, boost::function<void (int, void *)> reply_fn_obj);
	~Work_item();

public:
	void process();
	void reply();

private:
	boost::function<int (void *)> m_work_fn_obj;
	void *m_arg;

	boost::function<void (int, void *)> m_reply_fn_obj;
	int m_success;

	Task *m_ptask;
};

#endif