/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Program.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 14:47:51 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/14 16:19:12 by nessayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Program/Program.hpp"

Program::Program(void) {
}

Program::~Program(void) {
}

int				Program::checkErrorConfig(void) {
	return (0);
}

void			Program::parseConfig(std::string path) {
	std::string							line;
	std::vector<std::string>			lines;
	std::ifstream						file(path);

	if (this->checkErrorConfig() == 1)
		throw IncorrectConfigFileException();
	else
	{
		std::cout << "Parsing conf file at path "<< path << "." << std::endl;
		while (std::getline(file, line))
			lines.push_back(line);
	}
	file.close();
}

void			Program::start(void)
{
	std::cout << "Starting server..." << std::endl;
	//port & host from parsing
	short port = 8080;
	std::string host = "127.0.0.1";
	//

	while (true)
	{
		
	}
}

void			Program::stop(void) {
	std::cout << "Stopping server..." << std::endl;
}

void			Program::setup(void)
{
	server_socket = 
	FD_ZERO(&_current_sockets);
	FD_SET(server_socket)

}

const char*		Program::IncorrectConfigFileException::what() const throw()
{
	return "Config file is incorrect";
}
