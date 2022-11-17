#include "client.h"


int main()
{
	Infotecs_client client(8888, "127.0.0.1");

	std::thread thread1(&Infotecs_client::connect, std::ref(client));

	client.SetMessage();

	thread1.join();

	return 0;
}