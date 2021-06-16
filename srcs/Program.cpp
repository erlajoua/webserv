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
	if (this->isFieldMultiple(field, "methods") == true)
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
		else if (this->isFieldMultiple(*it, "methods") == true)
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
		throw 1;
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
					{
						throw 4;
						return;
					}
					else
						it++;
				}
				else
					it++;
			}	
			if (this->isClosingBracket(*it) == false)
			{
				throw 3;
				return;
			}
		}
		else if (this->isEmptyLine(*it) == false)
		{
			throw 2;
			return;
		}
	}
	std::cout << BOLDGREEN << "\n=> OK! Continuing parsing...\n" << RESET << std::endl;
}

void			Program::checkErrorConfig(std::vector<std::string> lines) {
	try
	{
		this->checkInvalidInstruction(lines);
	}
	catch (int e)
	{
		throw e;
		return;
	}
	try
	{
		this->checkInvalidDeclaration(lines);
	}
	catch (int e)
	{
		throw e;
		return;
	}
}

Server			Program::setServField(Server s, std::string const &field) {
	if (this->isFieldSingle(field, "port") == true)
	{
		try
		{
			s.setPort(field);
		}
		catch (int e)
		{
			throw 5;
			return (s);
		}
	}
	else if (this->isFieldSingle(field, "host") == true)
	{
		try
		{
			s.setHost(field);
		}
		catch (int e)
		{
			throw 6;
			return (s);
		}
	}
	else if (this->isFieldSingle(field, "server_name") == true)
	{
		try
		{
			s.setServerName(field);
		}
		catch (int e)
		{
			throw 7;
			return (s);
		}
	}
	else if (this->isFieldSingle(field, "root") == true)
	{
		try
		{
			s.setRoot(field);
		}
		catch (int e)
		{
			throw 8;
			return (s);
		}
	}
	else if (this->isFieldMultiple(field, "errors") == true)
	{
		try
		{
			s.setErrors(field);
		}
		catch (int e)
		{
			throw 9;
			return (s);
		}
	}
	else if (this->isFieldSingle(field, "client_body_size") == true)
	{
		try
		{
			s.setClientBodySize(field);
		}
		catch (int e)
		{
			throw 10;
			return (s);
		}
	}
	else if (this->isFieldSingle(field, "upload_dir") == true)
	{
		try
		{
			s.setUploadDir(field);
		}
		catch (int e)
		{
			throw 11;
			return (s);
		}
	}
	return (s);
}

Route			Program::setRouteField(Route r, std::string const &field) {
	if (this->isFieldSingle(field, "path") == true)
	{
		try
		{
			r.setPath(field);
		}
		catch (int e)
		{
			throw 12;
			return (r);
		}
	}
	if (this->isFieldMultiple(field, "methods") == true)
	{
		try
		{
			r.setMethods(field);
		}
		catch (int e)
		{
			throw 13;
			return (r);
		}
	}
	else if (this->isFieldSingle(field, "redirection") == true)
	{
		try
		{
			r.setRedirection(field);
		}
		catch (int e)
		{
			throw 14;
			return (r);
		}
	}
	else if (this->isFieldSingle(field, "autoindex") == true)
	{
		try
		{
			r.setAutoindex(field);
		}
		catch (int e)
		{
			throw 15;
			return (r);
		}
	}
	else if (this->isFieldSingle(field, "cgi_extension") == true)
	{
		try
		{
			r.setCgiExtension(field);
		}
		catch (int e)
		{
			throw 16;
			return (r);
		}
	}
	else if (this->isFieldSingle(field, "cgi_bin") == true)
	{
		try
		{
			r.setCgiBin(field);
		}
		catch (int e)
		{
			throw 17;
			return (r);
		}
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
			it++;
			while (this->isServField(*it) == true || this->isEmptyLine(*it) == true)
			{
				if (this->isServField(*it) == true)
				{
					try
					{
						s = this->setServField(s, *it);
					}
					catch (int e)
					{
						throw e;
						return;
					}
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
					it++;
					while (this->isRouteField(*it) == true || this->isEmptyLine(*it) == true)
					{
						if (this->isRouteField(*it) == true)
						{
							try
							{
								r = this->setRouteField(r, *it);
							}
							catch (int e)
							{
								throw e;
								return;
							}
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
	std::cout << BOLDGREEN << "\n=> OK! Starting program...\n" << RESET << std::endl;
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
	try
	{
		checkErrorConfig(lines);
	}
	catch (int e)
	{
		if (e == 1)
			throw InvalidInstructionException();
		else if (e == 2)
			throw ServerFirstException();
		else if (e == 3)
			throw InvalidServerFieldException();
		else if (e == 4)
			throw InvalidRouteFieldException();
		
		else if (e == 5)
			throw InvalidPortException();
		else if (e == 6)
			throw InvalidHostException();
		else if (e == 7)
			throw InvalidServerNameException();
		else if (e == 8)
			throw InvalidRootException();
		else if (e == 9)
			throw InvalidErrorsException();
		else if (e == 10)
			throw InvalidClientBodySizeException();
		else if (e == 11)
			throw InvalidUploadDirException();

		else if (e == 12)
			throw InvalidPathException();		
		else if (e == 13)
			throw InvalidMethodsException();
		else if (e == 14)
			throw InvalidRedirectionException();
		else if (e == 15)
			throw InvalidAutoindexException();
		else if (e == 16)
			throw InvalidCgiExtensionException();
		else if (e == 17)
			throw InvalidCgiBinDirException();
		return;
	}
	try
	{
		parseValue(lines);
	}
	catch (int e)
	{
		return;
	}
}

void			Program::printSetup(void) {
	std::cout << BOLDYELLOW << "Program has started with " << this->servers.size() << " active server(s):" << RESET << std::endl;
	for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		std::cout << YELLOW << "Server name = " << RESET;
		std::cout << (*it).getServerName() << std::endl;

		std::cout << YELLOW << "\tPort = " << RESET;
		std::cout << (*it).getPort() << std::endl;		

		std::cout << YELLOW << "\tHost = " << RESET;
		std::cout << (*it).getHost() << std::endl;

		std::cout << YELLOW << "\tRoot = " << RESET;
		std::cout << (*it).getRoot() << std::endl;

		std::cout << YELLOW << "\tNumber of error pages = " << RESET;
		std::cout << (*it).getErrors().size() << std::endl;

		std::cout << YELLOW << "\tMaximum client body size = " << RESET;
		std::cout << (*it).getClientBodySize() << std::endl;

		std::cout << YELLOW << "\tUpload directory = " << RESET;
		std::cout << (*it).getUploadDir() << std::endl;

		std::cout << YELLOW << "\tNumber of routes = " << RESET;
		std::cout << (*it).getRoutes()->size() << std::endl;

		std::cout << std::endl;
	}
}

void			Program::start(void) {
	while (1)
	{
	}
}

void			Program::stop(void) {
	std::cout << BOLDYELLOW << "Stopping server(s)..." << RESET << std::endl << std::endl;
}

// EXCEPTIONS

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


const char*		Program::InvalidPortException::what() const throw()
{
	return "Config file is incorrect: port value is incorrect.";
}

const char*		Program::InvalidHostException::what() const throw()
{
	return "Config file is incorrect: host value is incorrect.";
}

const char*		Program::InvalidServerNameException::what() const throw()
{
	return "Config file is incorrect: server_name value is incorrect.";
}

const char*		Program::InvalidRootException::what() const throw()
{
	return "Config file is incorrect: root value is incorrect.";
}

const char*		Program::InvalidErrorsException::what() const throw()
{
	return "Config file is incorrect: any of errors value is incorrect.";
}

const char*		Program::InvalidClientBodySizeException::what() const throw()
{
	return "Config file is incorrect: client_body_size value is incorrect.";
}

const char*		Program::InvalidUploadDirException::what() const throw()
{
	return "Config file is incorrect: upload value is incorrect.";
}


const char*		Program::InvalidPathException::what() const throw()
{
	return "Config file is incorrect: upload value is incorrect.";
}

const char*		Program::InvalidMethodsException::what() const throw()
{
	return "Config file is incorrect: upload value is incorrect.";
}

const char*		Program::InvalidRedirectionException::what() const throw()
{
	return "Config file is incorrect: upload value is incorrect.";
}

const char*		Program::InvalidAutoindexException::what() const throw()
{
	return "Config file is incorrect: upload value is incorrect.";
}

const char*		Program::InvalidCgiExtensionException::what() const throw()
{
	return "Config file is incorrect: upload value is incorrect.";
}

const char*		Program::InvalidCgiBinDirException::what() const throw()
{
	return "Config file is incorrect: upload value is incorrect.";
}

