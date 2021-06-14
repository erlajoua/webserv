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


# define BUFFER_SIZE 4096



void ft_error(std::string error)
{
	std::cout << "Error at " << error << "\n";
	exit(EXIT_FAILURE);
}

void *handle_connection(int client_socket)
{
	char buffToRead[4096];
	bzero(buffToRead, 4096);
	size_t bytes_read;
	char part2[1024];
	bzero(part2, 1024);

	//

	bytes_read = read(client_socket, buffToRead, 4096);
	std::cout << "Voici la requête : \n" << buffToRead << '\n';

	// 

	std::cout << "Write something (as h1)\n";

	bytes_read = read(0, part2, 1024);
	char part1[1024] = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:";
	char *tmp = new char(50);
	sprintf(tmp, "%zu", bytes_read + 9);
	strcat(part1, tmp);
	strcat(part1, "\n\n<h1>");
	part2[bytes_read - 1 ] = 0;
	strcat(part1, part2);
	strcat(part1, "</h1>");
	send(client_socket, part1, sizeof(part1), 0);
	bzero(part1, 1024);
	free(tmp);

	return NULL;
}

int accept_new_connection(int server_socket)
{
	int addr_size = sizeof(sockaddr_in);
	int client_socket;
	sockaddr_in client_addr;
	client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size);
	if (client_socket == -1)
		ft_error("Accept");
	return client_socket;
}

int setup_server(short port, int backlog)
{
	int server_socket;
	sockaddr_in server_addr;

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		ft_error("Socket");
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		ft_error("Bind");
	if (listen(server_socket, backlog) < 0)
		ft_error("Listen");
	return server_socket;
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << "Enter a port\n";
		return 1;
	}
	short port = (short)atoi(av[1]);
	int server_socket = setup_server(port, 3);


	fd_set current_sockets, ready_sockets;
	FD_ZERO(&current_sockets);
	FD_SET(server_socket, &current_sockets);

	while (true)
	{
		/*int client_socket = accept_new_connection(server_socket);

		handle_connection(client_socket);*/

		//because select is destructive
		ready_sockets = current_sockets;

		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
			ft_error("Select");
		
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &ready_sockets))
			{
				if (i == server_socket)
				{
					//new connection
					int client_socket = accept_new_connection(server_socket);
					FD_SET(client_socket, &current_sockets);
				}
				else
				{
					//std::cout << "passage\n";
					handle_connection(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}
		std::cout << "Execution du reste du programe....=====\n\n";
	} //while
	return 0;
}
