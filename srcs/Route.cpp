/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 13:34:11 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/15 13:34:25 by nessayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Route.hpp"

// PRIVATE HELPERS

bool				Route::isMethodDeclared(std::vector<std::string> methods, std::string word) {
	for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++)
	{
		if (*it == word)
			return (true);
	}
	return (false);
}


// CONSTRUCTOR & DESTRUCTOR

Route::Route(void)
	:path(std::string("none")), redirection(std::string("none")), autoindex(false),
	cgi_extension(std::string("none")), cgi_bin(std::string("none")) {
}

Route::Route(Route const &r) {
	this->path = r.path;
	this->methods = r.methods;
	this->redirection = r.redirection;
	this->autoindex = r.autoindex;
	this->cgi_extension = r.cgi_extension;
	this->cgi_bin = r.cgi_bin;	
}

Route::~Route(void) {
}

// OPERATOR

Route				&Route::operator=(Route const &r) {
	this->path = r.path;
	this->methods = r.methods;
	this->redirection = r.redirection;
	this->autoindex = r.autoindex;
	this->cgi_extension = r.cgi_extension;
	this->cgi_bin = r.cgi_bin;
	return (*this);
}

// GETTERS
std::string					Route::getPath(void) const {
	return (this->path);
}

std::vector<std::string>	Route::getMethods(void) const {
	return (this->methods);
}

std::string					Route::getRedirection(void) const {
	return (this->redirection);
}

bool						Route::getAutoindex(void) const {
	return (this->autoindex);
}

std::string					Route::getCgiExtension(void) const {
	return (this->cgi_extension);
}

std::string					Route::getCgiBin(void) const {
	return (this->cgi_bin);
}

// SETTERS

void				Route::setPath(std::string const &field) {
	size_t 	i = 0;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split[1][0] != '/')
		throw 12;
	else
		this->path = split[1];
}

void				Route::setMethods(std::string const &field) {
	size_t 	i = 0;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	size_t 	l = split.size();
	for (size_t j = 1; j < l; j++)
	{
		if (split[j] != "get" && split[j] != "post" && split[j] != "delete")
		{
			throw 13;
			return;
		}
		else if (this->isMethodDeclared(this->methods, split[j]))
		{
			throw 21;
			return;
		}
		else
			this->methods.push_back(split[j]);
	}
}

void				Route::setRedirection(std::string const &root, std::string const &field) {
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
	std::string concat = root + "/" + split[1];
	if (stat (concat.c_str(), &buffer) != 0)
		throw 14;
	else
		this->redirection = split[1];
}

void				Route::setAutoindex(std::string const &field) {
	size_t 	i = 0;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split[1] != "on" && split[1] != "off")
		throw 15;
	else
	{
		if (split[1] == "on")
			this->autoindex = true;
		else
			this->autoindex = false;
	}
}

void				Route::setCgiExtension(std::string const &field) {
	size_t 	i = 0;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split[1] == "none")
		throw 16;
	else
		this->cgi_extension = split[1];
}

void				Route::setCgiBin(std::string const &field) {
	size_t 	i = 0;
	struct stat buffer;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (stat (split[1].c_str(), &buffer) != 0)
		throw 17;
	else
		this->cgi_bin = split[1];
}

