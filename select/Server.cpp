#include "Server.hpp"

Server::Server(void)
{
	Server tmp(static_cast<short>(DEFAULT_PORT), DEFAULT_HOST, DEFAULT_NBCLIENTMAX);

	_port = tmp._port;
	_host = tmp._host;
	_nbClientMax = tmp._nbClientMax;
	_serverSocket = tmp._serverSocket;
	_currentSockets = tmp._currentSockets;
	_readySockets = tmp._readySockets;
	_addr = tmp._addr;
}

Server::Server(short port, const char *host, int nbClientMax) :
_port(port),
_host(host),
_nbClientMax(nbClientMax)
{
	memset(&_addr, 0, sizeof(_addr));


    if ((_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		ft_error("Socket");

	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = inet_addr(_host);
	_addr.sin_port = htons(_port);

    FD_ZERO(&_currentSockets);
    FD_SET(_serverSocket, &_currentSockets);
    
	if (bind(_serverSocket, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
		perror("bind");

	if (listen(_serverSocket, _nbClientMax) < 0)
		perror("Listen");
    std::cout << "===SUCCESS===\n\n-> A server has been setup on: " << _host << ":" << _port <<"\n\n=============\n";
}

Server::~Server(void)
{
}

//

#include <errno.h>

void    Server::start(void)
{
    while (true)
    {
        _readySockets = _currentSockets;

		if (select(FD_SETSIZE, &_readySockets, NULL, NULL, NULL) < 0)
			ft_error("Select");
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &_readySockets))
			{
				if (i == _serverSocket)
				{
					int client_socket = accept_new_connection(_serverSocket);
					FD_SET(client_socket, &_currentSockets);
				}
				else
				{
					handle_connection(i);
					FD_CLR(i, &_currentSockets);
				}
			}
		}
    }
}

int     Server::accept_new_connection(int server_socket)
{
	int addr_size = sizeof(sockaddr_in);
	int client_socket;

	sockaddr_in client_addr;
	client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size);

	if (client_socket == -1)
		ft_error("Accept new connection");

	return client_socket;
}

void    Server::handle_connection(int client_socket)
{
	//Le code est très sale, il faudra l'enlever, c'est juste le bout de code
	//Qui sert à envoyer une page html, pour l'afficher...
	//
	char buffToRead[4096];
	bzero(buffToRead, 4096);
	size_t bytes_read;

	char part2[4096];
	bzero(part2, 4096);

	std::cout << "Enter name of the file (html)\n";
	int input;
	
	bytes_read = read(0, part2, 4096);
	part2[bytes_read - 1] = 0;
	input = open(part2, O_RDONLY);
	if (input == -1)
	{
		bytes_read = 24;
		char part1[4096] = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:";
		char *tmp = new char(50);
		sprintf(tmp, "%zu", bytes_read);
		strcat(part1, tmp);
		strcat(part1, "\n\n");
		strcat(part1, "The file doesn't exist");
		send(client_socket, part1, sizeof(part1), 0);
		bzero(part1, 4096);
		free(tmp);
	}
	else
	{
		char part1[4096] = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:";
		char allfile[4096];
		int fd = open(part2, O_RDONLY);
		int ret = read(fd, allfile, 4096);
		allfile[ret] = 0;

		char *tmp = new char(50);
		sprintf(tmp, "%zu", bytes_read + ret);
		strcat(part1, tmp);
		strcat(part1, "\n\n");
		strcat(part1, allfile);
		send(client_socket, part1, sizeof(part1), 0);
		bzero(part1, 4096);
		close(fd);
		free(tmp);
	}
	close(input);
}
