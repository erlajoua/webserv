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
	:path(std::string("none")), redirection(std::string("none")),
	cgi_extension(std::string("none")), cgi_bin(std::string("none")) {
}

Route::Route(Route const &r) {
	this->path = r.path;
	this->methods = r.methods;
	this->redirection = r.redirection;
	this->cgi_extension = r.cgi_extension;
	this->cgi_bin = r.cgi_bin;	
}

Route::~Route(void)
{
}

// OPERATOR

Route				&Route::operator=(Route const &r) {
	this->path = r.path;
	this->methods = r.methods;
	this->redirection = r.redirection;
	this->cgi_extension = r.cgi_extension;
	this->cgi_bin = r.cgi_bin;
	return (*this);
}

// GETTERS
std::string						Route::getPath(void) const {
	return (this->path);
}

std::vector<std::string> const	&Route::getMethods(void) const {
	return (this->methods);
}

std::string						Route::getRedirection(void) const {
	return (this->redirection);
}

std::string						Route::getCgiExtension(void) const {
	return (this->cgi_extension);
}

std::string						Route::getCgiBin(void) const {
	return (this->cgi_bin);
}

// SETTERS

void							Route::setPath(std::string const &field) {
	size_t 	i = 0;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split[1][0] != '/')
		throw InvalidPathException();
	else
		this->path = split[1];
}

void							Route::setMethods(std::string const &field) {
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
			throw InvalidMethodsException();
		else if (this->isMethodDeclared(this->methods, split[j]))
			throw SameMethodException();
		else
			this->methods.push_back(split[j]);
	}
}

void							Route::setRedirection(std::string const &root, std::string const &field) {
	if (root == "none")
		throw RootNoneException();
	size_t 	i = 0;
	struct stat buffer;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string concat = root + "/" + split[1];
	if (stat (concat.c_str(), &buffer) != 0)
		throw InvalidRedirectionException();
	else
		this->redirection = split[1];
}

void							Route::setCgiExtension(std::string const &field) {
	size_t 	i = 0;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split[1] == "none")
		throw InvalidCgiExtensionException();
	else
		this->cgi_extension = split[1];
}

void							Route::setCgiBin(std::string const &field) {
	size_t 	i = 0;
	struct stat buffer;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (stat (split[1].c_str(), &buffer) != 0)
		throw InvalidCgiBinDirException();
	else
		this->cgi_bin = split[1];
}

// EXCEPTIONS

const char*		Route::InvalidPathException::what() const throw()
{
	return "Config file is incorrect: route path must start by /.";
}

const char*		Route::InvalidMethodsException::what() const throw()
{
	return "Config file is incorrect: methods value can only be get, post or delete.";
}

const char*		Route::InvalidRedirectionException::what() const throw()
{
	return "Config file is incorrect: redirection value (concatenated with root/) is not a valid path";
}

const char*		Route::InvalidCgiExtensionException::what() const throw()
{
	return "Config file is incorrect: cgi_extension value can't be set to none.";
}

const char*		Route::InvalidCgiBinDirException::what() const throw()
{
	return "Config file is incorrect: cgi_bin value is not a valid executable.";
}

const char*		Route::RootNoneException::what() const throw()
{
	return "Config file is incorrect: errors or redirection path can't be declared when root is set to none.";
}

const char*		Route::SameMethodException::what() const throw()
{
	return "Config file is incorrect: multiple declarations of the same method.";
}
