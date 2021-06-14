// Server 

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

int main(int ac, char **av)
{
	short port = (short)atoi(av[ac - 1]);
	if (ac != 2)
	{
		std::cout << "Enter the port\n";
		return 1;
	}

	int socketServer = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addrServer;

	addrServer.sin_addr.s_addr = INADDR_ANY;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(port);

	bind(socketServer, (struct sockaddr *)&addrServer, sizeof(addrServer));
	std::cout << "Bind : " << socketServer << "\n";
	
	std::cout << "Listen\n";

	listen(socketServer, 3);
	sockaddr_in addrClient;
	socklen_t clientSize = sizeof(addrClient);
	int socketClient = accept(socketServer, (struct sockaddr *)&addrClient, &clientSize);
	std::cout << "Accepted\n";
	std::cout << "Client : " << socketClient << "\n";
	
	char buffer[1024] = "Bonjour";

	write(socketClient, buffer, strlen(buffer));	
	close(socketClient);
	close(socketServer);
	return 0;
}
