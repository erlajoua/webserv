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

Server::Server(void) {
	this->server_name = std::string("default_server");
}

Server::Server(Server const &s) {
	this->server_name = s.server_name;
}

Server::~Server(void) {
}

Server				&Server::operator=(Server const &s) {
	this->server_name = s.server_name;
	return (*this);
}

std::string			Server::getServer_name(void) {
	return (this->server_name);
}