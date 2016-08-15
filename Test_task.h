#ifndef __TEST_TASK_H__
#define __TEST_TASK_H__

#include "common/Task.h"

#include "task_type.h"

//just a test

class Test_task : public Task
{
public:
	Test_task();
	~Test_task();

public:
	int initial(void *arg, boost::function<void (int, void *)> result_cb_obj);

public:
	int work_fn(void *arg);
	void reply_fn(int success, void *arg);

protected:
	TEST_TASK_TYPE_struct *m_pTEST_TASK_TYPE_struct;

protected:
	void report_status(int errorno, char *buf);

protected:
	void just_test();
};

#endif