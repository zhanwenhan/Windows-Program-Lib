#include <iostream>

#include <boost/bind.hpp>

#include "common/Task_manager.h"
#include "component/Middle_server_task.h"

#include "Test_task.h"
#include "handle_sys_error.h"

Test_task::Test_task()
{
	this->m_pTEST_TASK_TYPE_struct = NULL;
	this->m_task_type = TEST_TASK_TYPE;
	this->m_task_discribe = TEST_TASK_TYPE_DISCRIBE;
}

Test_task::~Test_task()
{

}

int Test_task::initial(void *arg, boost::function<void (int, void *)> result_cb_obj)
{
	dev_log("in initial");
	this->m_result_cb_obj = result_cb_obj;
	this->m_pTEST_TASK_TYPE_struct = (TEST_TASK_TYPE_struct *)arg;
	g_pmiddle_server_task->put_work(this, boost::bind(&Test_task::work_fn, this, _1), (void *)1, boost::bind(&Test_task::reply_fn, this, _1, _2));
	return 0;
}

int Test_task::work_fn(void *arg)
{
	dev_log("in thread");
	std::cout << "ads" << (int)arg << std::endl;
	return 0;
}

void Test_task::reply_fn(int success, void *arg)
{
	if(!success)
		dev_log("back ok");
	else
		dev_log("back not ok");
	std::cout << "ads" << (int)arg << std::endl;
	this->report_status(0, "test task ok");
	this->just_test();
	g_task_manager.recycle(this);
}


void Test_task::report_status(int errorno, char *buf)
{
	OutputDebugString(buf);
	this->m_result_cb_obj(errorno, this->m_pTEST_TASK_TYPE_struct);
}



void Test_task::just_test()
{

}

