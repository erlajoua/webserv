#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"

//constructors

Server::Server(void)
{
	Server tmp(static_cast<short>(DEFAULT_PORT), DEFAULT_HOST, DEFAULT_NB_CLIENT_MAX);

	this->port = tmp.port;
	this->host = tmp.host;
	this->nb_client_max = tmp.nb_client_max;
	this->server_socket = tmp.server_socket;
	this->current_sockets = tmp.current_sockets;
	this->ready_sockets = tmp.ready_sockets;
	this->addr = tmp.addr;
}

Server::Server(short port, const char *host, int nb_client_max) :
port(port), host(host), nb_client_max(nb_client_max)
{
	memset(&this->addr, 0, sizeof(this->addr));

    if ((this->server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		ftError("Socket");

	this->addr.sin_family = AF_INET;
	this->addr.sin_addr.s_addr = inet_addr(this->host);
	this->addr.sin_port = htons(this->port);

    FD_ZERO(&this->current_sockets);
    FD_SET(this->server_socket, &this->current_sockets);
    
	if (bind(this->server_socket, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0)
		ftError("bind (port maybe already use)");

	if (listen(this->server_socket, this->nb_client_max) < 0)
		ftError("Listen");
    std::cout << "===SUCCESS===\n\n-> A server has been setup on: " << this->host << ":" << this->port <<"\n\n=============\n";
}

//destructors

Server::~Server(void)
{
}

//methods

void    Server::start(void)
{
    while (true)
    {
        this->ready_sockets = this->current_sockets;

		if (select(FD_SETSIZE, &this->ready_sockets, NULL, NULL, NULL) < 0)
			ftError("Select");
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &this->ready_sockets))
			{
				if (i == this->server_socket)
				{
					int client_socket = acceptNewConnection(this->server_socket);
					FD_SET(client_socket, &this->current_sockets);
				}
				else
				{
					handleConnection(i);
					FD_CLR(i, &this->current_sockets);
				}
			}
		}
    }
}

int     Server::acceptNewConnection(int server_socket) const
{
	int addr_size = sizeof(sockaddr_in);
	int client_socket;

	sockaddr_in client_addr;
	client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size);

	if (client_socket == -1)
		ftError("Accept new connection");

	return (client_socket);
}

void    Server::handleConnection(int client_socket)
{
	//get the request content from the client_socket
	char request_buffer[4096];

	int bytesRead = read(client_socket, request_buffer, 4096);
	request_buffer[bytesRead] = 0;

	//create the request
	std::string request_content (request_buffer);
	Request request(request_content);

	std::cout << request;
}
