/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Program.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 14:47:51 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/15 12:56:07 by nessayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Program.hpp"

// PRIVATE HELPERS

bool			Program::isServConfig(std::string const &line) const {
	std::istringstream iss(line);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split.size() == 2 && split[0] == "server" && split[1] == "{")
		return (true);
	else
		return (false);
}

bool			Program::isRouteConfig(std::string const &line) const {
	std::istringstream iss(line);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split.size() == 3 && split[0] == "route" && split[2] == "{")
		return (true);
	else
		return (false);
}

bool			Program::isFieldSingle(std::string const &line, std::string const &field) const {
	(void) field;
	size_t 	i = 0;
	while(line[i] != '\0' && line[i] != ';')
		i++;
	if (line[i] == '\0')
		return (false);
	std::string up_to_colon(line, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split.size() == 2 && split[0] == field)
		return (true);
	else
		return (false);
}

bool			Program::isFieldMultiple(std::string const &line, std::string const &field) const {
	(void) field;
	size_t 	i = 0;
	while(line[i] != '\0' && line[i] != ';')
		i++;
	if (line[i] == '\0')
		return (false);
	std::string up_to_colon(line, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split.size() > 1 && split[0] == field)
		return (true);
	else
		return (false);
}

bool			Program::isField3(std::string const &line, std::string const &field) const {
	(void) field;
	size_t 	i = 0;
	while(line[i] != '\0' && line[i] != ';')
		i++;
	if (line[i] == '\0')
		return (false);
	std::string up_to_colon(line, 0, i);
	std::istringstream iss(up_to_colon);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split.size() > 1 && split.size() < 5 && split[0] == field)
		return (true);
	else
		return (false);
}

bool			Program::isClosingBracket(std::string const &line) const {
	std::istringstream iss(line);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split.size() == 1 && split[0] == "}")
		return (true);
	else
		return (false);
}

bool			Program::isEmptyLine(std::string const &line) const {
	std::istringstream iss(line);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split.size() == 0)
		return (true);
	else
		return (false);
}

bool			Program::isServField(std::string const &field) const {
	if (this->isFieldSingle(field, "port") == true)
		return (true);
	else if (this->isFieldSingle(field, "host") == true)
		return (true);
	else if (this->isFieldSingle(field, "server_name") == true)
		return (true);
	else if (this->isFieldSingle(field, "root") == true)
		return (true);
	else if (this->isFieldMultiple(field, "errors") == true)
		return (true);
	else if (this->isFieldSingle(field, "client_body_size") == true)
		return (true);
	else if (this->isFieldSingle(field, "upload_dir") == true)
		return (true);
	else
		return (false);
}

bool			Program::isRouteField(std::string const &field) const {
	if (this->isField3(field, "methods") == true)
		return (true);
	else if (this->isFieldSingle(field, "redirection") == true)
		return (true);
	else if (this->isFieldSingle(field, "autoindex") == true)
		return (true);
	else if (this->isFieldSingle(field, "cgi_extension") == true)
		return (true);
	else if (this->isFieldSingle(field, "cgi_bin") == true)
		return (true);
	else
		return (false);	
}

void			Program::checkNoServer(std::vector<std::string> lines) {
	std::cout << BOLDYELLOW << "Checking at least one server is declared..." << RESET << std::endl;
	usleep(1000000);
	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		if (this->isServConfig(*it) == true)
		{
			std::cout << BOLDGREEN << "\n=> OK! Continuing parsing...\n" << RESET << std::endl;
			return;
		}
	}
	throw NoServerException();
}

void			Program::checkInvalidInstruction(std::vector<std::string> lines) {
	bool	test = true;

	std::cout << BOLDYELLOW << "Checking for any invalid instruction..." << RESET << std::endl;
	usleep(1000000);
	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++) {
		std::cout << "---------------" << std::endl;
		std::cout << *it << std::endl;
		if (this->isServConfig(*it) == true)
			std::cout << GREEN << "*** Serv block starts ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "port") == true)
			std::cout << GREEN << "*** Port single field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "host") == true)
			std::cout << GREEN << "*** Host single field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "server_name") == true)
			std::cout << GREEN << "*** Server Name single field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "root") == true)
			std::cout << GREEN << "*** Root single field ***" << RESET << std::endl;
		else if (this->isFieldMultiple(*it, "errors") == true)
			std::cout << GREEN << "*** Errors multiple field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "client_body_size") == true)
			std::cout << GREEN << "*** Client Body Size single field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "upload_dir") == true)
			std::cout << GREEN << "*** Upload Directory single field ***" << RESET << std::endl;
		else if (this->isRouteConfig(*it) == true)
			std::cout << GREEN << "*** Route block starts ***" << RESET << std::endl;
		else if (this->isField3(*it, "methods") == true)
			std::cout << GREEN << "*** Methods multiple field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "redirection") == true)
			std::cout << GREEN << "*** Redirection single field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "autoindex") == true)
			std::cout << GREEN << "*** Auto single field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "cgi_extension") == true)
			std::cout << GREEN << "*** CGI Extension single field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "cgi_bin") == true)
			std::cout << GREEN << "*** CGI Directory single field ***" << RESET << std::endl;
		else if (this->isClosingBracket(*it) == true)
			std::cout << GREEN << "*** Closing bracket ***" << RESET << std::endl;
		else if (this->isEmptyLine(*it) == true)
			std::cout << GREEN << "*** Empty line ***" << RESET << std::endl;
		else
		{
			std::cout << RED << "*** Unrecognized line ***" << RESET << std::endl;
			test = false;
		}
		usleep(10000);
	}
	if (test == true)
		std::cout << BOLDGREEN << "\n=> OK! Continuing parsing...\n" << RESET << std::endl;
	else
		throw InvalidInstructionException();
}

void			Program::checkInvalidDeclaration(std::vector<std::string> lines) {
	std::cout << BOLDYELLOW << "Checking for any invalid declaration..." << RESET << std::endl;
	usleep(1000000);
	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		if (this->isServConfig(*it) == true)
		{
			std::cout << GREEN << "Checking next server declaration..." << RESET << std::endl;
			usleep(10000);
			it++;
			while (this->isServField(*it) == true || this->isEmptyLine(*it) == true)
				it++;
			while (this->isRouteConfig(*it) == true || this->isEmptyLine(*it) == true)
			{
				if (this->isRouteConfig(*it) == true)
				{
					std::cout << GREEN << "\tChecking next route declaration..." << RESET << std::endl;
					usleep(10000);
					it++;
					while (this->isRouteField(*it) == true || this->isEmptyLine(*it) == true)
						it++;
					if (this->isClosingBracket(*it) == false)
						throw InvalidRouteFieldException();
					else
						it++;
				}
				else
					it++;
			}	
			if (this->isClosingBracket(*it) == false)
				throw InvalidServerFieldException();
		}
		else if (this->isEmptyLine(*it) == false)
			throw ServerFirstException();
	}
	std::cout << BOLDGREEN << "\n=> OK! Continuing parsing...\n" << RESET << std::endl;
}

void			Program::checkErrorConfig(std::vector<std::string> lines) {
		this->checkNoServer(lines);
		this->checkInvalidInstruction(lines);
		this->checkInvalidDeclaration(lines);
}

Server			Program::setServField(Server s, std::string const &field) {
	if (this->isFieldSingle(field, "port") == true)
	{
		s.setPort(field);
	}
	else if (this->isFieldSingle(field, "host") == true)
	{
		s.setHost(field);
	}
	else if (this->isFieldSingle(field, "server_name") == true)
	{
		s.setServerName(field);
	}
	else if (this->isFieldSingle(field, "root") == true)
	{
		s.setRoot(field);
	}
	else if (this->isFieldMultiple(field, "errors") == true)
	{
		s.setErrors(s.getRoot(), field);
	}
	else if (this->isFieldSingle(field, "client_body_size") == true)
	{
		s.setClientBodySize(field);
	}
	else if (this->isFieldSingle(field, "upload_dir") == true)
	{
		s.setUploadDir(field);
	}
	return (s);
}

Route			Program::setRouteField(Server s, Route r, std::string const &field) {
	if (this->isField3(field, "methods") == true)
	{
		r.setMethods(field);
	}
	else if (this->isFieldSingle(field, "redirection") == true)
	{
		r.setRedirection(s.getRoot(), field);
	}
	else if (this->isFieldSingle(field, "autoindex") == true)
	{
		r.setAutoindex(field);
	}
	else if (this->isFieldSingle(field, "cgi_extension") == true)
	{
		r.setCgiExtension(field);
	}
	else if (this->isFieldSingle(field, "cgi_bin") == true)
	{
		r.setCgiBin(field);
	}
	return (r);
}

void			Program::parseValue(std::vector<std::string> lines) {
	std::cout << BOLDYELLOW << "Parsing valid values..." << RESET << std::endl;
	usleep(1000000);
	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		if (this->isServConfig(*it) == true)
		{
			std::cout << GREEN << "Parsing next server..." << RESET << std::endl;
			usleep(10000);
			Server s;
			if (this->servers.size() == 0)
				s.setDefaultServer();
			it++;
			while (this->isServField(*it) == true || this->isEmptyLine(*it) == true)
			{
				if (this->isServField(*it) == true)
				{
					s = this->setServField(s, *it);
				}
				it++;
			}
			while (this->isRouteConfig(*it) == true || this->isEmptyLine(*it) == true)
			{
				if (this->isRouteConfig(*it) == true)
				{
					std::cout << GREEN << "\tParsing next route..." << RESET << std::endl;
					usleep(10000);
					Route r;
					r.setPath(*it);
					it++;
					while (this->isRouteField(*it) == true || this->isEmptyLine(*it) == true)
					{
						if (this->isRouteField(*it) == true)
						{
							r = this->setRouteField(s, r, *it);
						}
						it++;
					}
					s.getRoutes()->push_back(r);
					it++;
				}
				else
					it++;
			}	
			this->servers.push_back(s);
		}
	}
	std::cout << BOLDGREEN << "\n=> OK! Continuing parsing...\n" << RESET << std::endl;
}

void			Program::checkMinimumSetup(void) {
	std::cout << BOLDYELLOW << "Checking minimum setup..." << RESET << std::endl;
	usleep(1000000);
	for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		std::cout << GREEN << "Checking next server..." << RESET << std::endl;
		if ((*it).getPort() == 0)
			throw NoPortSetupException();
		if ((*it).getHost() == "none")
			throw NoHostSetupException();
	}
	std::cout << BOLDGREEN << "\n=> OK! " << this->servers.size() << " server(s) have been successfully parsed!\n" << RESET << std::endl;
}


// CONSTRUCTOR & DESTRUCTOR

Program::Program(void) {
}

Program::~Program(void) {
}

// MEMBER FUNCTIONS

void			Program::parseConfig(std::string path) {
	std::string							line;
	std::vector<std::string>			lines;
	std::ifstream						file(path.c_str());

	std::cout << BOLDYELLOW << "\nParsing conf file at path "<< path << "...\n" << RESET << std::endl;
	while (std::getline(file, line))
		lines.push_back(line);
	file.close();
	usleep(1000000);
	this->checkErrorConfig(lines);
	this->parseValue(lines);
	this->checkMinimumSetup();
}

void			Program::printParsing(void) {
	std::cout << BOLDYELLOW << "Printing parsed server(s)..." << RESET << std::endl;
	usleep(1000000);
	for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		std::cout << YELLOW << "Server name = " << RESET;
		std::cout << (*it).getServerName() << std::endl;

		std::cout << YELLOW << "\tDefault server = " << RESET;
		if ((*it).getDefaultServer() == true)
			std::cout << "yes" << std::endl;
		else	
			std::cout << "no" << std::endl;

		std::cout << YELLOW << "\tPort = " << RESET;
		std::cout << (*it).getPort() << std::endl;		

		std::cout << YELLOW << "\tHost = " << RESET;
		std::cout << (*it).getHost() << std::endl;

		std::cout << YELLOW << "\tRoot = " << RESET;
		std::cout << (*it).getRoot() << std::endl;

		std::cout << YELLOW << "\tNumber of error pages = " << RESET;
		std::cout << (*it).getErrors().size() << std::endl;

		for (std::vector<std::string>::iterator it2 = (*it).getErrors().begin(); it2 != (*it).getErrors().end(); it2++)
		{
			std::cout << YELLOW << "\t\tPath = " << RESET;
			std::cout << *it2 << std::endl;
		}

		std::cout << YELLOW << "\tMaximum client body size = " << RESET;
		std::cout << (*it).getClientBodySize() << std::endl;

		std::cout << YELLOW << "\tUpload directory = " << RESET;
		std::cout << (*it).getUploadDir() << std::endl;

		std::cout << YELLOW << "\tNumber of routes = " << RESET;
		std::cout << (*it).getRoutes()->size() << std::endl;

		for (std::vector<Route>::iterator it3 = (*it).getRoutes()->begin(); it3 != (*it).getRoutes()->end(); it3++)
		{
			std::cout << YELLOW << "\t\tPath = " << RESET;
			std::cout << (*it3).getPath() << std::endl;

			std::cout << YELLOW << "\t\t\tMethods = " << RESET;
			for (std::vector<std::string>::iterator it4 = (*it3).getMethods().begin(); it4 != (*it3).getMethods().end(); it4++)
			{
				std::cout << (*it4) << " ";				
			}
			std::cout << std::endl;

			std::cout << YELLOW << "\t\t\tRedirection = " << RESET;
			std::cout << (*it3).getRedirection() << std::endl;

			std::cout << YELLOW << "\t\t\tAutoindex = " << RESET;
			if ((*it3).getAutoindex() == true)
				std::cout << "on" << std::endl;
			else		
				std::cout << "off" << std::endl;

			std::cout << YELLOW << "\t\t\tCGI extension = " << RESET;
			std::cout << (*it3).getCgiExtension() << std::endl;

			std::cout << YELLOW << "\t\t\tCGI executable = " << RESET;
			std::cout << (*it3).getCgiBin() << std::endl;
		}

		std::cout << std::endl;
	}
}

void			Program::setup(void) {

	std::cout << BOLDYELLOW << "Setting up " << this->servers.size() << " servers..." << RESET << std::endl;
	for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		(*it).setup();
	}
	std::cout << std::endl;
}

void			Program::start(void) {

	std::cout << BOLDYELLOW << "Starting " << this->servers.size() << " servers..." << RESET << std::endl;
	for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		(*it).start();
	}
	std::cout << std::endl;
}

void			Program::stop(void) {
	std::cout << BOLDYELLOW << "Stopping server(s)..." << RESET << std::endl << std::endl;
}

// EXCEPTIONS

const char*		Program::NoServerException::what() const throw()
{
	return "Config file is incorrect: no server declared.";
}

const char*		Program::InvalidInstructionException::what() const throw()
{
	return "Config file is incorrect: invalid instruction(s).";
}

const char*		Program::ServerFirstException::what() const throw()
{
	return "Config file is incorrect: each block must start by a server declaration.";
}

const char*		Program::InvalidServerFieldException::what() const throw()
{
	return "Config file is incorrect: invalid field within a server declaration or a } is missing.";
}

const char*		Program::InvalidRouteFieldException::what() const throw()
{
	return "Config file is incorrect: invalid field within a route declaration or a } is missing.";
}

const char*		Program::NoPortSetupException::what() const throw()
{
	return "Config file is incorrect: a port must be declared for each server.";
}

const char*		Program::NoHostSetupException::what() const throw()
{
	return "Config file is incorrect: a host must be declared for each server.";
}