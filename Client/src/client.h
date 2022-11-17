#pragma once
#include <iostream>
#include<string>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <algorithm>
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

class Client 
{
protected:
#ifdef _WIN32
	WSAData _wsa;
#endif
	SOCKET _clientsocket;
	std::string _ipaddress;
	int _port;
	std::string _message;
	struct sockaddr_in _client_info;
	int _infolength;
public:
	Client(int, std::string);
	~Client();

virtual	void connect();
virtual	void SetMessage();
protected:
	void init();

virtual	void send();

};
/////////////////////////////////////////////////////////////////////////////////////////////////////////
class Infotecs_client:public Client
{
private:
	std::string _buffer;
	std::mutex _mut;
	std::condition_variable _var;
	bool _status;
	int _sum;

public:
	Infotecs_client(int, std::string);

	void connect() override;
	void SetMessage() override;
	
private:
	void before_sending();
	void send() override;
};