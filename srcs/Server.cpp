/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 13:33:37 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/29 15:13:31 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// CONSTRUCTOR & DESTRUCTOR

Server::Server(void)
	:default_server(false), port(0), host(std::string("none")), server_name(std::string("none")),
	root(std::string("none")), client_body_size(0), server_socket(-1)
{
		Location	l;
		l.setPath("location / {");
		this->locations.push_back(l);
}

Server::Server(Server const &s)
{
	this->default_server = s.default_server;
	this->port = s.port;
	this->host = s.host;
	this->server_name = s.server_name;
	this->root = s.root;
	this->errors = s.errors;
	this->client_body_size = s.client_body_size;
	this->locations = s.locations;
	this->addr = s.addr;
	this->server_socket = s.server_socket;
}

Server::~Server(void)
{
}

// OPERATOR

Server						&Server::operator=(Server const &s)
{
	this->default_server = s.default_server;
	this->port = s.port;
	this->host = s.host;
	this->server_name = s.server_name;
	this->root = s.root;
	this->errors = s.errors;
	this->client_body_size = s.client_body_size;
	this->locations = s.locations;
	this->addr = s.addr;
	this->server_socket = s.server_socket;
	return (*this);
}

// GETTERS

bool							Server::getDefaultServer(void) const {
	return (this->default_server);
}

int								Server::getPort(void) const {
	return (this->port);
}

std::string						Server::getHost(void) const {
	return (this->host);
}

std::string						Server::getServerName(void) const {
	return (this->server_name);
}

std::string						Server::getRoot(void) const {
	return (this->root);
}

std::vector<std::string> const&	Server::getErrors(void) const {
	return (this->errors);
}

size_t							Server::getClientBodySize(void) const {
	return (this->client_body_size);
}

std::vector<Location>				*Server::getLocations(void)
{
	return (&(this->locations));
}

int							Server::getServerSocket(void) const
{
	return (this->server_socket);
}

// SETTERS

void							Server::setDefaultServer(void)
{
	this->default_server = true;
}

void							Server::setPort(std::string const &field)
{
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
	ret = std::atoi(split[1].c_str());
	if (ret < 1 || ret > 65535)
		throw InvalidPortException();
	else
		this->port = ret;
}

void							Server::setHost(std::string const &field)
{
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

void							Server::setServerName(std::string const &field)
{
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

void							Server::setRoot(std::string const &field)
{
	size_t 	i = 0;
	struct stat buffer;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if ((stat (split[1].c_str(), &buffer) != 0) || split[1][split[1].size() - 1] == '/')
		throw InvalidRootException();
	else
		this->root = split[1];
}

void							Server::setErrors(std::string const &root, std::string const &field)
{
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

void							Server::setClientBodySize(std::string const &field)
{
	size_t 	i = 0;
	int 	ret;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	ret = std::atoi(split[1].c_str());
	if (ret < 1 || ret > 2147483646)
		throw InvalidClientBodySizeException();
	else
		this->client_body_size = ret;
}

// MEMBER FUNCTIONS

void	Server::handleRequest(int client_socket,
		std::string const &request_content, Request &request, char **envp) {
	std::cout << BOLDBLUE << "===[" << this->server_name << "] <-- RECEIVED REQUEST FROM SOCKET n°[" << client_socket << "]===" RESET << std::endl;
	std::cout << BLUE << request_content.substr(0, 1000);
	if (request_content.size() > 1000)
		std::cout << std::endl << "[TRUNCATED]";
	std::cout << RESET << std::endl;

	Response response(envp, request, *this);
	std::string response_content(response.toString());
	if (send(client_socket, response_content.c_str(), response_content.length(), 0) == -1)
	{
		std::cerr << RED << "send system call failed" << RESET << std::endl;
		request.setConnection(kClose);
		return;
	}
	std::cout << BOLDGREEN << "===[" << this->server_name << "] --> SENT RESPONSE TO SOCKET n°[" << client_socket << "]===" RESET << std::endl;
	std::cout << GREEN << response_content.substr(0, 300);
	if (response_content.size() > 300)
		std::cout << std::endl << "[TRUNCATED]";
	std::cout << RESET << std::endl;
}

int     Server::acceptNewConnection() const
{
	int addr_size = sizeof(sockaddr_in);
	int client_socket;

	sockaddr_in client_addr;
	client_socket = accept(this->server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size);

	if (client_socket == -1)
		throw AcceptNewConnectionException();

	return (client_socket);
}

void							Server::setup(void)
{
	if ((this->server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw SocketInitializationException();
	memset(&this->addr, 0, sizeof(this->addr));
	this->addr.sin_family = AF_INET;
	this->addr.sin_addr.s_addr = inet_addr(this->host.c_str());
	this->addr.sin_port = htons(this->port);
	if (bind(this->server_socket, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0)
		throw BindException();
	if (listen(this->server_socket, NB_CLIENT_MAX) < 0)
		throw ListenException();
	std::cout << GREEN << this->getServerName() << " is setup on socket " << this->server_socket << "." << RESET << std::endl;
	//usleep(10000);
}

// EXCEPTIONS

const char*		Server::SocketInitializationException::what() const throw()
{
	return "Server instanciation error: invalid socket creation.";
}

const char*		Server::InvalidPortException::what() const throw()
{
	return "Config file is incorrect: port value must be 0 < int < 2147483646.";
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
	return "Config file is incorrect: root value is not a valid path or end with /.";
}

const char*		Server::InvalidErrorsException::what() const throw()
{
	return "Config file is incorrect: any of errors value (concatenated with root/) is not a valid path.";
}

const char*		Server::InvalidClientBodySizeException::what() const throw()
{
	return "Config file is incorrect: client_body_size value must be 0 < int < 65535.";
}

const char*		Server::BindException::what() const throw()
{
	return "Server setup error: can't bind (can't connect to host or port may be already used).";
}

const char*		Server::ListenException::what() const throw()
{
	return "Server setup error: can't listen.";
}

const char*		Server::AcceptNewConnectionException::what() const throw()
{
	return "Server runtime error: can't accept new connection.";
}
