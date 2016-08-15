#ifndef __TIME_VALUE_H__
#define __TIME_VALUE_H__

#include <WinSock2.h>

class Time_value
{
public:
	Time_value();
	Time_value(long sec, long usec);
	~Time_value();

public:
	//set time value by second and usecond
	void set(long sec, long usec);
	//set time value by timeval
	void set(struct timeval *tv);
	long get_sec();
	long get_usec();
	void get_timeval(struct timeval *tv);
	struct timeval get_timeval();
	bool operator == (const Time_value &tv) const;
	Time_value &operator = (const Time_value &tv);

private:
	struct timeval m_tv;
};

#endif
