/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Program.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 14:47:51 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/29 15:18:12 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Program.hpp"

// PRIVATE HELPERS

bool			Program::isServConfig(std::string const &line) const {
	std::istringstream iss(line);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split.size() == 2 && split[0] == "server" && split[1] == "{")
		return (true);
	else
		return (false);
}

bool			Program::isLocationConfig(std::string const &line) const {
	std::istringstream iss(line);
	std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (split.size() == 3 && split[0] == "location" && split[2] == "{")
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
	else
		return (false);
}

bool			Program::isLocationField(std::string const &field) const {
	if (this->isField3(field, "methods") == true)
		return (true);
	else if (this->isFieldSingle(field, "redirection") == true)
		return (true);
	else if (this->isFieldSingle(field, "index") == true)
		return (true);
	else if (this->isFieldSingle(field, "cgi_extension") == true)
		return (true);
	else if (this->isFieldSingle(field, "cgi_bin") == true)
		return (true);
	else if (this->isFieldSingle(field, "autoindex") == true)
		return (true);
	else if (this->isFieldSingle(field, "upload_dir") == true)
		return (true);
	else
		return (false);	
}

void			Program::checkNoServer(std::vector<std::string> lines) {
	std::cout << BOLDYELLOW << "Checking at least one server is declared..." << RESET << std::endl;
	//usleep(1000000);
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
	//usleep(1000000);
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
		else if (this->isLocationConfig(*it) == true)
			std::cout << GREEN << "*** Location block starts ***" << RESET << std::endl;
		else if (this->isField3(*it, "methods") == true)
			std::cout << GREEN << "*** Methods multiple field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "redirection") == true)
			std::cout << GREEN << "*** Redirection single field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "index") == true)
			std::cout << GREEN << "*** Index single field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "autoindex") == true)
			std::cout << GREEN << "*** Autoindex single field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "cgi_extension") == true)
			std::cout << GREEN << "*** CGI Extension single field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "cgi_bin") == true)
			std::cout << GREEN << "*** CGI Directory single field ***" << RESET << std::endl;
		else if (this->isFieldSingle(*it, "upload_dir") == true)
			std::cout << GREEN << "*** Upload Directory single field ***" << RESET << std::endl;
		else if (this->isClosingBracket(*it) == true)
			std::cout << GREEN << "*** Closing bracket ***" << RESET << std::endl;
		else if (this->isEmptyLine(*it) == true)
			std::cout << GREEN << "*** Empty line ***" << RESET << std::endl;
		else
		{
			std::cout << RED << "*** Unrecognized line ***" << RESET << std::endl;
			test = false;
		}
		//usleep(10000);
	}
	if (test == true)
		std::cout << BOLDGREEN << "\n=> OK! Continuing parsing...\n" << RESET << std::endl;
	else
		throw InvalidInstructionException();
}

void			Program::checkInvalidDeclaration(std::vector<std::string> lines) {
	std::cout << BOLDYELLOW << "Checking for any invalid declaration..." << RESET << std::endl;
	//usleep(1000000);
	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		if (this->isServConfig(*it) == true)
		{
			std::cout << GREEN << "Checking next server declaration..." << RESET << std::endl;
			//usleep(10000);
			it++;
			while (this->isServField(*it) == true || this->isEmptyLine(*it) == true)
				it++;
			while (this->isLocationConfig(*it) == true || this->isEmptyLine(*it) == true)
			{
				if (this->isLocationConfig(*it) == true)
				{
					std::cout << GREEN << "\tChecking next location declaration..." << RESET << std::endl;
					//usleep(10000);
					it++;
					while (this->isLocationField(*it) == true || this->isEmptyLine(*it) == true)
						it++;
					if (this->isClosingBracket(*it) == false)
						throw InvalidLocationFieldException();
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
	return (s);
}

Location			Program::setLocationField(Server s, Location r, std::string const &field) {
	if (this->isField3(field, "methods") == true)
	{
		r.setMethods(field);
	}
	else if (this->isFieldSingle(field, "redirection") == true)
	{
		r.setRedirection(field);
	}
	else if (this->isFieldSingle(field, "index") == true)
	{
		r.setIndex(s.getRoot(), field);
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
		r.setCgiBin(s.getRoot(), field);
	}
	else if (this->isFieldSingle(field, "upload_dir") == true)
	{
		r.setUploadDir(s.getRoot(), field);
	}
	return (r);
}

void			Program::parseValue(std::vector<std::string> lines) {
	std::cout << BOLDYELLOW << "Parsing valid values..." << RESET << std::endl;
	//usleep(1000000);
	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		if (this->isServConfig(*it) == true)
		{
			std::cout << GREEN << "Parsing next server..." << RESET << std::endl;
			//usleep(10000);
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
			while (this->isLocationConfig(*it) == true || this->isEmptyLine(*it) == true)
			{
				if (this->isLocationConfig(*it) == true)
				{
					std::cout << GREEN << "\tParsing next location..." << RESET << std::endl;
					//usleep(10000);
					std::istringstream iss(*it);
					std::vector<std::string> split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
					Location r;
					r.setPath(*it);
					it++;
					while (this->isLocationField(*it) == true || this->isEmptyLine(*it) == true)
					{
						if (this->isLocationField(*it) == true)
						{
							r = this->setLocationField(s, r, *it);
						}
						it++;
					}
					if (split[1] != "/")
						s.getLocations()->push_back(r);
					else
						(*s.getLocations())[0] = r;
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
	//usleep(1000000);
	for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		std::cout << GREEN << "Checking next server..." << RESET << std::endl;
		if ((*it).getPort() == 0)
			throw NoPortSetupException();
		if ((*it).getHost() == "none")
			throw NoHostSetupException();
		if ((*it).getRoot() == "none")
			throw NoRootSetupException();
		for (std::vector<Location>::iterator it2 = (*it).getLocations()->begin(); it2 != (*it).getLocations()->end(); it2++)
		{
			if ((*it2).getCgiExtension() != "none" && (*it2).getCgiBin() == "none")
				throw NoCgiBinException();
		}
		//usleep(10000);
	}
	std::cout << BOLDGREEN << "\n=> OK! " << this->servers.size() << " server(s) have been successfully parsed!\n" << RESET << std::endl;
}

bool			Program::hasAlreadyDefaultServer(std::vector<Server>::iterator iter) {
	for (std::vector<Server>::iterator it = this->servers.begin(); it != iter; it++)
	{
		if ((*it).getHost() == (*iter).getHost() && (*it).getPort() == (*iter).getPort())
			return true;
	}
	return false;
}

void			Program::setDefaultServer(void) {
	for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		if (hasAlreadyDefaultServer(it) == false)
			(*it).setDefaultServer();
	}
}

void			Program::setDefaultRoutes(void) {
	for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		for (std::vector<Location>::iterator it2 =(*it).getLocations()->begin(); it2 != (*it).getLocations()->end(); it2++)
		if ((*it2).getMethods().size() == 0)
		{
			std::string m1("get");
			std::string m2("post");
			std::string m3("delete");
			(*it2).getMethods().push_back(m1);
			(*it2).getMethods().push_back(m2);
			(*it2).getMethods().push_back(m3);			
		}
	}
}

void			Program::setupEnvp(char **main_envp)
{
	char **main_envp_it = main_envp;
	while (*main_envp_it)
	{
		main_envp_it++;
	}
	this->envp = new char *[main_envp_it - main_envp + 7 + 1];
	main_envp_it = main_envp;
	char **envp_it = this->envp;
	while (envp_it - this->envp < 7)
	{
		*envp_it = NULL;
		envp_it++;
	}
	while (*main_envp_it)
	{
		*envp_it = *main_envp_it;
		envp_it++;
		main_envp_it++;
	}
	*envp_it = NULL;
}

void			Program::acceptNewServerConnection(int server_socket) {
	for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); ++it)
	{
		if (server_socket == it->getServerSocket())
		{
			int client_socket = it->acceptNewConnection();
			fcntl(client_socket, F_SETFL, O_NONBLOCK);
			FD_SET(client_socket, &this->readfds);
			FD_SET(client_socket, &this->writefds);
			break;
		}
	}
}

void			Program::handleRequest(int client_socket) {
	char request_buffer[4096];
	std::string request_content = this->pending_requests_content[client_socket];
	int bytesRead = recv(client_socket, request_buffer, 4096, 0);
	if (bytesRead == -1)
	{
		std::cerr << RED << "recv system call failed" << RESET << std::endl;
		close(client_socket);
		FD_CLR(client_socket, &this->readfds);
		FD_CLR(client_socket, &this->writefds);
		this->pending_requests_content.erase(client_socket);
		return;
	}
	request_content += std::string(request_buffer, bytesRead);
	Request request(request_content);
	if (request.getIsFinished() == false)
	{
		this->pending_requests_content[client_socket] = request_content;
		return;
	}
	this->pending_requests_content.erase(client_socket);

	std::vector<Server>::iterator request_server_it = this->servers.begin();
	for (std::vector<Server>::iterator it = ++this->servers.begin(); it != this->servers.end(); ++it)
	{
		if (request.getPort() == it->getPort())
		{
			if (request.getHost() == it->getServerName())
			{
				request_server_it = it;
				break;
			}
			else if (it->getDefaultServer() == true)
			{
				request_server_it = it;
			}
		}
	}
	request_server_it->handleRequest(client_socket, request_content, request, this->envp);
	if (request.getConnection() == kClose)
	{
		close(client_socket);
		FD_CLR(client_socket, &this->readfds);
		FD_CLR(client_socket, &this->writefds);
	}
}

void			Program::httpServerIO(void) {
	fd_set ready_readfds = this->readfds;
	fd_set ready_writefds = this->writefds;

	if (select(FD_SETSIZE, &ready_readfds, &ready_writefds, NULL, NULL) == -1)
	{
		if (errno == EINTR)
		{
			return;
		}
		else
		{
			throw SelectException();
		}
	}
	for (int i = 0; i < FD_SETSIZE; i++)
	{
		if (FD_ISSET(i, &ready_readfds))
		{
			if (!FD_ISSET(i, &this->writefds))
			{
				this->acceptNewServerConnection(i);
			}
			else if (FD_ISSET(i, &ready_writefds))
			{
				this->handleRequest(i);
			}
		}
	}
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
	//usleep(1000000);
	this->checkErrorConfig(lines);
	this->parseValue(lines);
	this->checkMinimumSetup();
	this->setDefaultServer();
	this->setDefaultRoutes();
}

void			Program::printParsing(void) {
	std::cout << BOLDYELLOW << "Printing parsed server(s)..." << RESET << std::endl;
	//usleep(1000000);
	for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		std::cout << YELLOW << "Server name = " << RESET;
		std::cout << (*it).getServerName() << std::endl;

		std::cout << YELLOW << "\tPort = " << RESET;
		std::cout << (*it).getPort() << std::endl;		

		std::cout << YELLOW << "\tHost = " << RESET;
		std::cout << (*it).getHost() << std::endl;

		std::cout << YELLOW << "\tDefault server = " << RESET;
		if ((*it).getDefaultServer() == true)
			std::cout << "yes" << std::endl;
		else	
			std::cout << "no" << std::endl;

		std::cout << YELLOW << "\tRoot = " << RESET;
		std::cout << (*it).getRoot() << std::endl;

		std::cout << YELLOW << "\tNumber of error pages = " << RESET;
		std::cout << (*it).getErrors().size() << std::endl;

		for (std::vector<std::string>::const_iterator it2 = (*it).getErrors().begin(); it2 != (*it).getErrors().end(); it2++)
		{
			std::cout << YELLOW << "\t\tPath = " << RESET;
			std::cout << *it2 << std::endl;
		}

		std::cout << YELLOW << "\tMaximum client body size = " << RESET;
		std::cout << (*it).getClientBodySize() << std::endl;

		std::cout << YELLOW << "\tNumber of locations = " << RESET;
		std::cout << (*it).getLocations()->size() << std::endl;

		for (std::vector<Location>::iterator it3 = (*it).getLocations()->begin(); it3 != (*it).getLocations()->end(); it3++)
		{
			std::cout << YELLOW << "\t\tPath = " << RESET;
			std::cout << (*it3).getPath() << std::endl;

			std::cout << YELLOW << "\t\t\tMethods = " << RESET;
			if ((*it3).getMethods().size() == 0)
				std::cout << "none";
			else
			{
				for (std::vector<std::string>::const_iterator it4 = (*it3).getMethods().begin(); it4 != (*it3).getMethods().end(); it4++)
				{
					std::cout << (*it4) << " ";
				}
			}
			std::cout << std::endl;

			std::cout << YELLOW << "\t\t\tRedirection = " << RESET;
			std::cout << (*it3).getRedirection() << std::endl;

			std::cout << YELLOW << "\t\t\tIndex = " << RESET;
			std::cout << (*it3).getIndex() << std::endl;

			std::cout << YELLOW << "\t\t\tAutoindex = " << RESET;
			if ((*it3).getAutoindex() == true)
				std::cout << "on" << std::endl;
			else		
				std::cout << "off" << std::endl;

			std::cout << YELLOW << "\t\t\tCGI extension = " << RESET;
			std::cout << (*it3).getCgiExtension() << std::endl;

			std::cout << YELLOW << "\t\t\tCGI executable = " << RESET;
			std::cout << (*it3).getCgiBin() << std::endl;

			std::cout << YELLOW << "\t\t\tUpload directory = " << RESET;
			std::cout << (*it3).getUploadDir() << std::endl;

		}
		//usleep(1000000);
		std::cout << std::endl;
	}
}

void			Program::setup(char **envp) {
	FD_ZERO(&this->readfds);
	FD_ZERO(&this->writefds);
	std::cout << BOLDYELLOW << "Setting up " << this->servers.size() << " servers..." << RESET << std::endl;
	//usleep(1000000);
	for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{
		if (it->getDefaultServer() == true)
		{
			it->setup();
			fcntl(it->getServerSocket(), F_SETFL, O_NONBLOCK);
			FD_SET(it->getServerSocket(), &this->readfds);
		}
		//usleep(10000);
	}
	this->setupEnvp(envp);
	std::cout << std::endl;
}

void			Program::start(void) {

	std::cout << BOLDYELLOW << "Starting " << this->servers.size() << " server(s)..." << RESET << std::endl;

	this->is_running = true;
	for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); ++it)
	{
		std::cout << GREEN << it->getServerName() << " is now listening on " << it->getHost() << ":" << it->getPort() << "..." << RESET << std::endl << std::endl;
	}
	//usleep(10000);
	while (this->is_running == true)
	{
		try
		{
			this->httpServerIO();
		}
		catch (std::exception &e)
		{
			std::cerr << RED << std::endl << "=> " << e.what() << RESET << std::endl << std::endl;
		}
	}
}

void			Program::stop(void) {
	std::cout << BOLDYELLOW << "Stopping " << this->servers.size() << " server(s)..." << RESET << std::endl;
	this->is_running = false;
	FD_ZERO(&this->readfds);
	for (std::vector<Server>::iterator it = this->servers.begin(); it != this->servers.end(); ++it)
	{
		if (it->getDefaultServer() == true)
		{
			close(it->getServerSocket());
		}
		std::cout << GREEN << it->getServerName() << " has stopped." << RESET << std::endl;
	}
	for (int i = 0; i < FD_SETSIZE; i++)
	{
		if (FD_ISSET(i, &this->writefds))
		{
			close(i);
		}
	}
	FD_ZERO(&this->writefds);
	delete [] this->envp;
	//usleep(1000000);
}

void			Program::setIsRunning(bool is_running_value) {
	this->is_running = is_running_value;
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

const char*		Program::InvalidLocationFieldException::what() const throw()
{
	return "Config file is incorrect: invalid field within a location declaration or a } is missing.";
}

const char*		Program::NoPortSetupException::what() const throw()
{
	return "Config file is incorrect: a port must be declared for each server.";
}

const char*		Program::NoHostSetupException::what() const throw()
{
	return "Config file is incorrect: a host must be declared for each server.";
}

const char*		Program::NoRootSetupException::what() const throw()
{
	return "Config file is incorrect: a root must be declared for each server.";
}

const char*		Program::NoCgiBinException::what() const throw()
{
	return "Config file is incorrect: a location is declared with a CGI extension but without CGI executable.";
}

const char*		Program::SelectException::what() const throw()
{
	return "Server runtime error: select error.";
}
