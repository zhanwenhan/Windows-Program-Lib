#ifndef __SOCK_ADDR_H__
#define __SOCK_ADDR_H__

#include <ws2def.h>

#include <string>

//wrap the sock addr

class Sock_addr
{
public:
	Sock_addr();
	~Sock_addr();

public:
	//set ip and port
	int set(std::string ip, int port = 0);
	//set ip and port by sockaddr_in
	int set_by_sockaddr_in(struct sockaddr_in *sin, int socklen);
	std::string get_ip();
	int get_port();
	void get_sockaddr_in(struct sockaddr_in *sin);
	struct sockaddr_in get_sockaddr_in();
	bool operator == (const Sock_addr &sa) const;
	Sock_addr &operator = (const Sock_addr &sa);

private:
	std::string m_ip_string;
	int m_port;
	struct sockaddr_in m_sin;
};

#endif