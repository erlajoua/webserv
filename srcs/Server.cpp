/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 13:33:37 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/19 10:17:56 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

// PRIVATE HELPERS

int     Server::acceptNewConnection(int server_socket) const
{
	int addr_size = sizeof(sockaddr_in);
	int client_socket;

	sockaddr_in client_addr;
	client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size);

	if (client_socket == -1)
		throw AcceptNewConectionException();

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
	std::cout << CYAN << std::endl << this->server_name << " received a request from client_socket " << client_socket << ":" RESET << std::endl;
	
	Response response(request);
	
	std::cout << "Response = " << response.toString() << "\n";
	write(client_socket, response.toString().c_str(), strlen(response.toString().c_str()));
}

// CONSTRUCTOR & DESTRUCTOR

Server::Server(void)
	:default_server(false), port(0), host(std::string("none")), server_name(std::string("none")),
	root(std::string("none")), client_body_size(0), upload_dir(std::string("none")), running(false) {
		memset(&this->addr, 0, sizeof(this->addr));
		if ((this->server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw SocketInitializationException();
		FD_ZERO(&this->current_sockets);
		FD_ZERO(&this->ready_sockets); //check;
}

Server::Server(Server const &s) {
	this->default_server = s.default_server;
	this->port = s.port;
	this->host = s.host;
	this->server_name = s.server_name;
	this->root = s.root;
	this->errors = s.errors;
	this->client_body_size = s.client_body_size;
	this->upload_dir = s.upload_dir;
	this->routes = s.routes;
	this->addr = s.addr;
	this->server_socket = s.server_socket;
	this->current_sockets = s.current_sockets;
	this->ready_sockets = s.ready_sockets;
	this->running = s.running;	
}

Server::~Server(void) {
}

// OPERATOR

Server				&Server::operator=(Server const &s) {
	this->default_server = s.default_server;
	this->port = s.port;
	this->host = s.host;
	this->server_name = s.server_name;
	this->root = s.root;
	this->errors = s.errors;
	this->client_body_size = s.client_body_size;
	this->upload_dir = s.upload_dir;
	this->routes = s.routes;
	this->addr = s.addr;
	this->server_socket = s.server_socket;
	this->current_sockets = s.current_sockets;
	this->ready_sockets = s.ready_sockets;
	this->running = s.running;
	return (*this);
}

// GETTERS

bool						Server::getDefaultServer(void) const {
	return (this->default_server);
}

int							Server::getPort(void) const {
	return (this->port);
}

std::string					Server::getHost(void) const {
	return (this->host);
}

std::string					Server::getServerName(void) const {
	return (this->server_name);
}

std::string					Server::getRoot(void) const {
	return (this->root);
}

std::vector<std::string> const&	Server::getErrors(void) const {
	return (this->errors);
}

int							Server::getClientBodySize(void) const {
	return (this->client_body_size);
}

std::string					Server::getUploadDir(void) const {
	return (this->upload_dir);
}

std::vector<Route>			*Server::getRoutes(void) {
	return (&(this->routes));
}

// SETTERS

void				Server::setDefaultServer(void) {
	this->default_server = true;
}

void				Server::setPort(std::string const &field) {
	size_t 	i = 0;
	int 	ret;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	for (size_t j = 0; j < split[1].length(); j++)
	{
		if (isdigit(split[1][j]) == false)
			throw InvalidPortException();
	}
	ret = atoi(split[1].c_str());
	if (ret < 1 || ret > 65535)
		throw InvalidPortException();
	else
		this->port = ret;
}

void				Server::setHost(std::string const &field) {
	size_t 	i = 0;
	int 	ret;
	struct sockaddr_in sa;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	ret = inet_pton(AF_INET, split[1].c_str(), &(sa.sin_addr));
	if (ret == 0)
		throw InvalidHostException();
	else
		this->host = split[1];
}

void				Server::setServerName(std::string const &field) {
	size_t 	i = 0;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split[1] == "none")
		throw InvalidServerNameException();
	else
		this->server_name = split[1];
}

void				Server::setRoot(std::string const &field) {
	size_t 	i = 0;
	struct stat buffer;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (stat (split[1].c_str(), &buffer) != 0)
		throw InvalidRootException();
	else
		this->root = split[1];
}

void				Server::setErrors(std::string const &root, std::string const &field) {
	if (root == "none")
		throw Route::RootNoneException();
	size_t 	i = 0;
	struct stat buffer;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	size_t 	l = split.size();
	std::string concat;
	for (size_t j = 1; j < l; j++)
	{
		concat = root + "/" + split[j];
		if (stat (concat.c_str(), &buffer) != 0)
			throw InvalidErrorsException();
		else
			this->errors.push_back(split[j]);
	}
}

void				Server::setClientBodySize(std::string const &field) {
	size_t 	i = 0;
	int 	ret;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	ret = atoi(split[1].c_str());
	if (ret < 1 || ret > 65535)
		throw InvalidClientBodySizeException();
	else
		this->client_body_size = ret;
}

void				Server::setUploadDir(std::string const &field) {
	size_t 	i = 0;
	struct stat buffer;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (stat (split[1].c_str(), &buffer) != 0)
		throw InvalidUploadDirException();
	else
		this->upload_dir = split[1];
}

// MEMBER FUNCTIONS

void				Server::setup(void) {
	this->addr.sin_family = AF_INET;
	this->addr.sin_addr.s_addr = inet_addr(this->host.c_str());
	this->addr.sin_port = htons(this->port);
    FD_SET(this->server_socket, &this->current_sockets);
	if (bind(this->server_socket, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0)
		throw BindException();
	if (listen(this->server_socket, NB_CLIENT_MAX) < 0)
		throw ListenException();
	std::cout << GREEN << this->getServerName() << " is setup on socket " << this->server_socket << "." << RESET << std::endl;
	//usleep(10000);
}

void				*Server::start(void *server_v) {
	Server	*server;
	server = reinterpret_cast<Server *>(server_v);

	server->running = true;
	std::cout << GREEN << server->getServerName() << " is now listening on " << server->getHost() << ":" << server->getPort() << "..." << RESET << std::endl;
	//usleep(10000);
	while (server->running == true)
	{
		try
		{
		    server->ready_sockets = server->current_sockets;

			if (select(FD_SETSIZE, &server->ready_sockets, NULL, NULL, NULL) < 0)
				throw SelectException();
			for (int i = 0; i < FD_SETSIZE; i++)
			{
				if (FD_ISSET(i, &server->ready_sockets))
				{
					if (i == server->server_socket)
					{
						int client_socket = server->acceptNewConnection(server->server_socket);
						FD_SET(client_socket, &server->current_sockets);
					}
					else
					{
						server->handleConnection(i);
						FD_CLR(i, &server->current_sockets);
					}
				}
			}
		}
		catch (std::exception &e)
		{
			std::cerr << RED << std::endl << "=> " << e.what() << RESET << std::endl << std::endl;
		}
	}
	return (NULL);
}

void				Server::stop(void) {
	this->running = false;
	std::cout << GREEN << this->getServerName() << " has stopped." << RESET << std::endl;
	//usleep(10000);
}

// EXCEPTIONS

const char*		Server::SocketInitializationException::what() const throw()
{
	return "Server instanciation error: invalid socket creation.";
}

const char*		Server::InvalidPortException::what() const throw()
{
	return "Config file is incorrect: port value must be 0 < int < 65535.";
}

const char*		Server::InvalidHostException::what() const throw()
{
	return "Config file is incorrect: host value is not a valid IPv4 address.";
}

const char*		Server::InvalidServerNameException::what() const throw()
{
	return "Config file is incorrect: server_name value can't be set to none.";
}

const char*		Server::InvalidRootException::what() const throw()
{
	return "Config file is incorrect: root value is not a valid path.";
}

const char*		Server::InvalidErrorsException::what() const throw()
{
	return "Config file is incorrect: any of errors value (concatenated with root/) is not a valid path.";
}

const char*		Server::InvalidClientBodySizeException::what() const throw()
{
	return "Config file is incorrect: client_body_size value must be 0 < int < 65535.";
}

const char*		Server::InvalidUploadDirException::what() const throw()
{
	return "Config file is incorrect: upload_dir value is not a valid path.";
}

const char*		Server::BindException::what() const throw()
{
	return "Server setup error: can't bind (port may be already used).";
}

const char*		Server::ListenException::what() const throw()
{
	return "Server setup error: can't listen.";
}

const char*		Server::AcceptNewConectionException::what() const throw()
{
	return "Server runtime error: can't accept new connection.";
}

const char*		Server::SelectException::what() const throw()
{
	return "Server runtime error: select error.";
}
