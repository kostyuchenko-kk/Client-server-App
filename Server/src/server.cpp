#include "server.h"

#ifdef __linux__
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif
Server::Server(int port, std::string ipaddress)
	:
	_port(port),
	_ipaddress(ipaddress),
	_serversocket(INVALID_SOCKET),
	_serv_info{ 0 },
	_infolength(sizeof(_serv_info)) {}

void Server::init()
{
	_serv_info.sin_family = AF_INET;
	_serv_info.sin_port = htons(_port);
	_serv_info.sin_addr.s_addr = inet_addr(_ipaddress.c_str());

#ifdef _WIN32

	if (WSAStartup(MAKEWORD(2, 2), &_wsa) != 0)
	{
		std::cout << "Error WinSock version initializaion" << std::endl;
		return;
	}
	std::cout << "WSA Success" << std::endl;
#endif


	if ((_serversocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		std::cout << "Error initialization socket" << std::endl;
		return;
	}

	std::cout << "Success socket initialization!" << std::endl;


	if (bind(_serversocket, (struct sockaddr*)&_serv_info, _infolength)!=0)
	{
		std::cout << "socket binding error" << std::endl;
		return;
	}
	
	std::cout << "Success socket binded" << std::endl;
	std::cout << "Server started at "<< inet_ntoa(_serv_info.sin_addr) << ntohs(_serv_info.sin_port)<< std::endl;

}

void Server::receive()
{
#ifdef _WIN32
	auto ref_infolenght = &_infolength;
#else
	auto ref_infolenght = (socklen_t*)&_infolength;
#endif
	if ((_recvlength = recvfrom(_serversocket, _buffer, SIZE, 0, (struct sockaddr*)&_serv_info, ref_infolenght)) == SOCKET_ERROR)
	{
		std::cout << "failed to receive message" << std::endl;

	}
}

void Server::proccess()
{
	std::cout << "Packet from " << inet_ntoa(_serv_info.sin_addr) << " " << ntohs(_serv_info.sin_port) << std::endl;
	std::cout << "Data: ";
		for (size_t i = 0; i < _recvlength; i++)
		{
			std::cout<<_buffer[i];
		}

		std::cout << std::endl;
}
void Server::start()
{
	init();


	while (1)
	{
		receive();
		proccess();
	}
}
Server::~Server()
{
#ifdef _WIN32
	WSACleanup();
	closesocket(_serversocket);
#else 
	close(_serversocket);
#endif

}
///////////////////////////////////////////////////////////////////
Infotecs_server::Infotecs_server(int port, std::string ipaddress)
	:
	Server::Server(port, ipaddress),
	_int_buff{ 0 }{}


void Infotecs_server::receive()
{
#ifdef _WIN32
	auto lenght = &_infolength;
#else
	auto lenght = (socklen_t*)&_infolength;
#endif

	if (recvfrom(_serversocket, (char*)&_int_buff, sizeof(int), 0, (struct sockaddr*)&_serv_info, lenght) == SOCKET_ERROR)
	{
		std::cout << "recv failed" << std::endl;

	}
}

void Infotecs_server::proccess()
{
	if (_int_buff > 99 && _int_buff % 32 == 0)
	{
		std::cout << "data recieved" << std::endl;
	}
	else
	{
		std::cout << "error:incorrect data" << std::endl;

	}
	

}
