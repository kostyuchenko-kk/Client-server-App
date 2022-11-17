#include "client.h"
#ifdef __linux__
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif
Client::Client(int port, std::string ipaddress)
	:
	_port(port),
	_ipaddress(ipaddress),
	_clientsocket(INVALID_SOCKET),
	_client_info{ 0 },
	_infolength(sizeof(_client_info)) {}

void Client::init()
{
	_client_info.sin_family = AF_INET;
	_client_info.sin_port = htons(_port);
	_client_info.sin_addr.s_addr = inet_addr(_ipaddress.c_str());

#ifdef _WIN32

	if (WSAStartup(MAKEWORD(2, 2), &_wsa) != 0)
	{
		std::cout << "Error WinSock version initializaion" << std::endl;
		return;
	}
	std::cout << "WSA Success" << std::endl;
#endif


	if ((_clientsocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		std::cout << "Error initialization socket" << std::endl;
		return;
	}

	std::cout << "Success socket initialization!" << std::endl;

}
 void Client:: SetMessage()
{
	 std::cout << "Enter a message" << std::endl;
	 std::getline(std::cin, _message);

}
void Client::send()
{
		if ((sendto(_clientsocket, _message.c_str(), _message.size(), 0, (struct sockaddr*)&_client_info, _infolength)) == SOCKET_ERROR)
		{
			std::cout << "send failed" << std::endl;
		}
	

}


void Client::connect()
{
	init();


	while (1)
	{
		SetMessage();
		send();

	}
}
Client::~Client()
{
#ifdef _WIN32
	WSACleanup();
	closesocket(_clientsocket);
#else 
	close(_clientsocket);
#endif

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Infotecs_client::Infotecs_client(int port, std::string ipaddress)
	:
	Client::Client(port,ipaddress),
	_status{false},
	_sum{0}{}


void Infotecs_client::SetMessage() 
{
	while (true)
	{
		std::string tmp_str;
		bool isSymbol = false;
		std::getline(std::cin, tmp_str);
		if (tmp_str.size() > 64 || tmp_str.empty())
		{
			std::cout << "invalid string size" << std::endl;
			continue;
		}
		for (const auto& el : tmp_str)
		{
			if (!std::isdigit(el))
			{
				std::cout << "string contains an element that is not a digit" << std::endl;
				isSymbol = true;
				break;
			}
		}
		if (isSymbol)
		{
			continue;
		}
		std::sort(tmp_str.begin(), tmp_str.end(), [](const auto& a, const auto& b)
			{
				return a > b;
			});
		std::lock_guard<std::mutex> lg(_mut);
		_buffer.reserve(tmp_str.size() * 2);
		for (const auto& el : tmp_str)
		{
			if (el % 2 == 0)
			{
				_buffer += "KB";
			}
			else
			{
				_buffer.push_back(el);
			}
		}
		_status = true;
		_var.notify_one();

	}
}

void Infotecs_client::before_sending()
{
	std::unique_lock<std::mutex> ul(_mut);
	_var.wait(ul, [=,this]() {return _status;});
	std::cout << _buffer << std::endl;
	for (const auto& el : _buffer)
	{
		if (std::isdigit(el))
		{
			_sum += el - '0';
		}

	}
	_buffer.clear();
	_status = false;
	ul.unlock();

}
void Infotecs_client::send()
{
	if ((sendto(_clientsocket, (char*)&_sum, sizeof(int), 0, (struct sockaddr*)&_client_info, sizeof(_client_info))) == SOCKET_ERROR)
	{
		std::cout << "send failed" << std::endl;
	}
	_sum = 0;

}
void Infotecs_client::connect()
{
	init();


	while (1)
	{
		before_sending();
		send();

	}

}