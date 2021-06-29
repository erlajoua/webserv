/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 17:47:12 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/24 12:00:59 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

// PRIVATE HELPERS

bool				Location::isMethodDeclared(std::vector<std::string> methods, std::string word) {
	for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++)
	{
		if (*it == word)
			return (true);
	}
	return (false);
}


// CONSTRUCTOR & DESTRUCTOR

Location::Location(void)
	:path(std::string("none")), redirection(301), index(std::string("none")), autoindex(false),
	cgi_extension(std::string("none")), cgi_bin(std::string("none")), upload_dir("none") {
}

Location::Location(Location const &r) {
	this->path = r.path;
	this->redirection = r.redirection;
	this->methods = r.methods;
	this->index = r.index;
	this->autoindex = r.autoindex;
	this->cgi_extension = r.cgi_extension;
	this->cgi_bin = r.cgi_bin;	
	this->upload_dir = r.upload_dir;
}

Location::~Location(void)
{
}

// OPERATOR

Location				&Location::operator=(Location const &r) {
	this->path = r.path;
	this->redirection = r.redirection;
	this->methods = r.methods;
	this->index = r.index;
	this->autoindex = r.autoindex;
	this->cgi_extension = r.cgi_extension;
	this->cgi_bin = r.cgi_bin;
	this->upload_dir = r.upload_dir;
	return (*this);
}

// GETTERS
std::string						Location::getPath(void) const {
	return (this->path);
}

int								Location::getRedirection(void) const {
	return (this->redirection);
}

std::vector<std::string> 		&Location::getMethods(void) {
	return (this->methods);
}

std::string						Location::getIndex(void) const {
	return (this->index);
}

bool							Location::getAutoindex(void) const {
	return (this->autoindex);
}

std::string						Location::getCgiExtension(void) const {
	return (this->cgi_extension);
}

std::string						Location::getCgiBin(void) const {
	return (this->cgi_bin);
}

std::string						Location::getUploadDir(void) const
{
	return (this->upload_dir);
}

// SETTERS

void				Location::setPath(std::string const &field) {
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

void				Location::setMethods(std::string const &field) {
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

void				Location::setRedirection(std::string const &field)
{
	size_t 	i = 0;
	int 	ret;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	ret = std::atoi(split[1].c_str());
	if (ret != 301 && ret != 302 && ret != 303 && ret != 304 && ret != 307 && ret != 308)
		throw InvalidRedirectionException();
	else
		this->redirection = ret;
}

void				Location::setIndex(std::string const &root, std::string const &field) {
	(void)root;
	size_t 	i = 0;
	//struct stat buffer;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	/*std::string concat = root + "/" + split[1];
	if (stat (concat.c_str(), &buffer) != 0)
		throw InvalidIndexException();
	else*/
	this->index = split[1];
}

void				Location::setAutoindex(std::string const &field) {
	size_t 	i = 0;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split[1] != "on" && split[1] != "off")
		throw InvalidAutoindexException();
	else
	{
		if (split[1] == "on")
			this->autoindex = true;
		else
			this->autoindex = false;
	}
}

void				Location::setCgiExtension(std::string const &field) {
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

void				Location::setCgiBin(std::string const &root, std::string const &field) {
	size_t 	i = 0;
	struct stat buffer;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string concat;
	if (this->path == "/")
		concat = root + this->path + split[1];
	else
		concat = root + this->path + "/" + split[1];
	if (stat (concat.c_str(), &buffer) != 0)
		throw InvalidCgiBinDirException();
	else
		this->cgi_bin = split[1];
}

void				Location::setUploadDir(std::string const &root, std::string const &field)
{
	size_t 	i = 0;
	struct stat buffer;
	while(field[i] != '\0' && field[i] != ';')
		i++;
	std::string up_to_colon(field, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string concat;
	if (this->path == "/")
		concat = root + this->path + split[1];
	else
		concat = root + this->path + "/" + split[1];
	if (stat (concat.c_str(), &buffer) != 0)
		throw InvalidUploadDirException();
	else
		this->upload_dir = split[1];
}

// EXCEPTIONS

const char*		Location::InvalidPathException::what() const throw()
{
	return "Config file is incorrect: Location path must start by /.";
}

const char*		Location::InvalidMethodsException::what() const throw()
{
	return "Config file is incorrect: methods value can only be get, post or delete.";
}

const char*		Location::SameMethodException::what() const throw()
{
	return "Config file is incorrect: multiple declarations of the same method.";
}

const char*		Location::InvalidRedirectionException::what() const throw()
{
	return "Config file is incorrect: redirection value must be 301, 302, 303, 304, 307 or 308";
}
/*
const char*		Location::InvalidIndexException::what() const throw()
{
	return "Config file is incorrect: index value (concatenated with root/) is not a valid path";
}
*/
const char*		Location::InvalidAutoindexException::what() const throw()
{
	return "Config file is incorrect: autoindex value can only be on or off.";
}

const char*		Location::InvalidCgiExtensionException::what() const throw()
{
	return "Config file is incorrect: cgi_extension value can't be set to none.";
}

const char*		Location::InvalidCgiBinDirException::what() const throw()
{
	return "Config file is incorrect: cgi_bin value (concatenated with root/location/) is not a valid executable.";
}

const char*		Location::InvalidUploadDirException::what() const throw()
{
	return "Config file is incorrect: upload_dir value (concatenated with root/location/) is not a valid path.";
}
