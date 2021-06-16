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

Server::Server(void) {
	this->server_name = std::string("default_server");
}

Server::Server(Server const &s) {
	this->server_name = s.server_name;
}

Server::~Server(void) {
}

// OPERATOR

Server				&Server::operator=(Server const &s) {
	this->server_name = s.server_name;
	return (*this);
}

// GETTERS

std::string			Server::getServerName(void) const {
	return (this->server_name);
}

std::vector<Route>	Server::getRoutes(void) const {
	return (this->routes);
}

// SETTERS

void				Server::setPort(std::string const &field) {
	(void)field;
}

void				Server::setHost(std::string const &field) {
	(void)field;
}

void				Server::setServerName(std::string const &field) {
	(void)field;
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
