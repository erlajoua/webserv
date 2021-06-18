/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 13:33:37 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/15 13:34:00 by nessayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

// CONSTRUCTOR & DESTRUCTOR

Server::Server(void)
	:default_server(false), port(0), host(std::string("none")), server_name(std::string("none")),
	root(std::string("none")), client_body_size(0), upload_dir(std::string("none")) {
		memset(&this->addr, 0, sizeof(this->addr));
		this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
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

std::vector<std::string>	Server::getErrors(void) const {
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
		{
			throw 5;
			return;
		}
	}
	ret = atoi(split[1].c_str());
	if (ret < 1 || ret > 65535)
		throw 5;
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
		throw 6;
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
		throw 7;
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
		throw 8;
	else
		this->root = split[1];
}

void				Server::setErrors(std::string const &root, std::string const &field) {
	if (root == "none")
	{
		throw 20;
		return;
	}
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
		{
			throw 9;
			return;
		}
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
		throw 10;
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
		throw 11;
	else
		this->upload_dir = split[1];
}

// MEMBER FUNCTIONS

void				Server::setup(void) {
	this->addr.sin_family = AF_INET;
	this->addr.sin_addr.s_addr = inet_addr(this->host.c_str());
	this->addr.sin_port = htons(this->port);
    FD_SET(this->server_socket, &this->current_sockets);
	bind(this->server_socket, (struct sockaddr *)&this->addr, sizeof(this->addr));
	listen(this->server_socket, NB_CLIENT_MAX);

	std::cout << GREEN << this->getServerName() << " is setup on socket " << this->server_socket << "." << RESET << std::endl;
	usleep(10000);
}

void				Server::start(void) {
	std::cout << GREEN << this->getServerName() << " is now listening on " << this->getHost() << ":" << this->getPort() << "..." << RESET << std::endl;
	usleep(10000);
	while (1)
	{
	}
}