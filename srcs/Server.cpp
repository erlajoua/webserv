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
	:port(0), host(std::string("none")), server_name(std::string("none")),
	root(std::string("none")), client_body_size(0), upload_dir(std::string("none")) {

}

Server::Server(Server const &s) {
	this->port = s.port;
	this->host = s.host;
	this->server_name = s.server_name;
	this->root = s.root;
	this->errors = s.errors;
	this->client_body_size = s.client_body_size;
	this->upload_dir = s.upload_dir;
	this->routes = s.routes;
}

Server::~Server(void) {
}

// OPERATOR

Server				&Server::operator=(Server const &s) {
	this->port = s.port;
	this->host = s.host;
	this->server_name = s.server_name;
	this->root = s.root;
	this->errors = s.errors;
	this->client_body_size = s.client_body_size;
	this->upload_dir = s.upload_dir;
	this->routes = s.routes;
	return (*this);
}

// GETTERS

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

void				Server::setPort(std::string const &field) {
	(void)field;
}

void				Server::setHost(std::string const &field) {
	(void)field;
}

void				Server::setServerName(std::string const &field) {
	size_t 	i = 0;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	this->server_name = split[1];
}

void				Server::setRoot(std::string const &field) {
	(void)field;
}

void				Server::setErrors(std::string const &field) {
	(void)field;
}

void				Server::setClientBodySize(std::string const &field) {
	(void)field;
}

void				Server::setUploadDir(std::string const &field) {
	(void)field;
}
