#include <event2/util.h>
#include "Time_value.h"

Time_value::Time_value()
{
	evutil_timerclear(&this->m_tv);
}

Time_value::Time_value(long sec, long usec)
{
	evutil_timerclear(&this->m_tv);
	this->m_tv.tv_sec = sec;
	this->m_tv.tv_usec = usec;
}

Time_value::~Time_value()
{

}

void Time_value::set(long sec, long usec)
{
	this->m_tv.tv_sec = sec;
	this->m_tv.tv_usec = usec;
}

void Time_value::set(struct timeval *tv)
{
	this->m_tv = *tv;
}

long Time_value::get_sec()
{
	return this->m_tv.tv_sec;
}

long Time_value::get_usec()
{
	return this->m_tv.tv_usec;
}

void Time_value::get_timeval(struct timeval *tv)
{
	tv->tv_sec = this->m_tv.tv_sec;
	tv->tv_usec = this->m_tv.tv_usec;
}

struct timeval Time_value::get_timeval()
{
	return this->m_tv;
}

bool Time_value::operator == (const Time_value &tv) const
{
	if( this->m_tv.tv_sec == tv.m_tv.tv_sec && this->m_tv.tv_usec == tv.m_tv.tv_usec)
		return true;
	else
		return false;
}

Time_value &Time_value::operator = (const Time_value &tv)
{
	this->m_tv.tv_sec = tv.m_tv.tv_sec;
	this->m_tv.tv_usec = tv.m_tv.tv_usec;
	return *this;
}

