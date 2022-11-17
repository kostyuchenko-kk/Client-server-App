#pragma once
#include <iostream>
#include<string>
#ifdef _WIN32
#include<WinSock2.h>
#pragma comment(lib,"Ws2_32.lib") 
#pragma warning(disable: 4996)

#else
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define SOCKET int
#endif

#define SIZE 512

class Server
{
protected:
#ifdef _WIN32
	WSAData _wsa;
#endif
	SOCKET _serversocket;
	std::string _ipaddress;
	int _port;
	char _buffer[SIZE];
	struct sockaddr_in _serv_info;
	int _infolength;
	int _recvlength;
public:
	Server(int, std::string);
	virtual ~Server();

virtual	void start();
protected:
	void init();
	virtual void receive();
	virtual void proccess();
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
class Infotecs_server:public Server
{
private:
	int _int_buff;
public:
	Infotecs_server(int, std::string);


private:
	void receive() override;
	void proccess()override;


};