#include "server.h"

int main()
{
	Infotecs_server server(8888, "127.0.0.1");
	server.start();
	return 0;
}