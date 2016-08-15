#include <WinSock2.h>
#include <windows.h>

#include <cstring>

#include <event2/util.h>

#include "Sock_addr.h"
#include "handle_error.h"

Sock_addr::Sock_addr()
{
	this->m_ip_string = "";
	this->m_port = 0;
	std::memset(&this->m_sin, 0, sizeof(struct sockaddr_in));
	this->m_sin.sin_family = AF_INET;
}

Sock_addr::~Sock_addr()
{

}

int Sock_addr::set(std::string ip, int port)
{
	this->m_ip_string = ip;
	this->m_port = port;
	std::memset(&this->m_sin, 0, sizeof(struct sockaddr_in));
	this->m_sin.sin_family = AF_INET;
	this->m_sin.sin_port = htons(port);
	if(evutil_inet_pton(AF_INET, ip.c_str(), &this->m_sin.sin_addr) <= 0)
	{
		handle_error("Sock_addr::set - IP Address error");
		return -1;
	}
	return 0;
}

int Sock_addr::set_by_sockaddr_in(struct sockaddr_in *sin, int socklen)
{
	char ip_buf[64];
	if(socklen != sizeof(struct sockaddr_in))
	{
		handle_error("Sock_addr::set_by_sockaddr_in - sock length error");
		return -1;
	}
	if(!evutil_inet_ntop(AF_INET, (const void *)&sin->sin_addr, ip_buf, sizeof(ip_buf)))
	{
		handle_error("Sock_addr::set_by_sockaddr_in - evutil_inet_ntop");
		return -1;
	}
	this->m_ip_string = ip_buf;
	this->m_port = ntohs(sin->sin_port);
	this->m_sin = *sin;

	return 0;
}

std::string Sock_addr::get_ip()
{
	return this->m_ip_string;
}

int Sock_addr::get_port()
{
	return this->m_port;
}

void Sock_addr::get_sockaddr_in(struct sockaddr_in *sin)
{
	std::memset(sin, 0, sizeof(struct sockaddr_in));
	sin->sin_family = this->m_sin.sin_family;
	sin->sin_addr = this->m_sin.sin_addr;
	sin->sin_port = this->m_sin.sin_port;
}

struct sockaddr_in Sock_addr::get_sockaddr_in()
{
	return this->m_sin;
}

bool Sock_addr::operator == (const Sock_addr &sa) const
{
	if(this->m_ip_string == sa.m_ip_string && this->m_port == sa.m_port)
		return true;
	else
		return false;
}

Sock_addr &Sock_addr::operator = (const Sock_addr &sa)
{
	this->m_ip_string = sa.m_ip_string;
	this->m_port = sa.m_port;
	this->m_sin = sa.m_sin;
	return *this;
}


