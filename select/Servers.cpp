#include "Servers.hpp"

Servers::Servers(void)
{
    Servers(static_cast<short>(DEFAULT_PORT), DEFAULT_HOST, DEFAULT_NBCLIENTMAX);
}

Servers::Servers(short port, const char *host, int nbClientMax) :
_port(port), _host(host), _nbClientMax(nbClientMax)
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
		ft_error("Bind (the port is maybe still used, change it while starting the program");
	if (listen(_serverSocket, _nbClientMax) < 0)
		ft_error("Listen");

    std::cout << "===SUCCESS===\n\n-> A server has been setup on: " << _host << ":" << _port <<"\n\n=============\n";
}

Servers::~Servers(void)
{
}

//

void    Servers::start(void)
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

int     Servers::accept_new_connection(int server_socket)
{
	int addr_size = sizeof(sockaddr_in);
	int client_socket;

	sockaddr_in client_addr;
	client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size);

	if (client_socket == -1)
		ft_error("Accept new connection");

	return client_socket;
}

void    *Servers::handle_connection(int client_socket)
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

	std::cout << "===Write something (as h1)===\n";

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