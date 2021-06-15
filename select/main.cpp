#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <limits.h>
#include <string.h>
#include <string>

#include "Servers.hpp"

void ft_error(std::string error)
{
	std::cout << "Error at " << error << "\n";
	exit(EXIT_FAILURE);
}

int main(int ac, char **av)
{
	if (ac != 2)
		ft_error("Please enter a port as argument");

	const char *host = DEFAULT_HOST;
	short port = (short)atoi(av[1]);
	int   nbClientMax = 3;

	//

	Servers serv1(port, host, nbClientMax);
	//Servers serv1;

	serv1.start();
	return 0;
}