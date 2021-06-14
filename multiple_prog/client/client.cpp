// Client


#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int ac, char **av)
{
	short port = (short)atoi(av[ac - 1]);
	if (ac != 2)
	{
		std::cout << "Enter the port\n";
		return 1;
	}

	int socketClient = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addrClient;
	addrClient.sin_addr.s_addr = INADDR_ANY;
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(port);
	connect(socketClient, (struct sockaddr *)&addrClient, sizeof(addrClient));
	std::cout << "Connected\n";

	char buffer[1024];

	read(socketClient, buffer, 1024);
	
	std::cout << "User receveid : " << buffer << "\n";
	close(socketClient);

	return 0;
}

