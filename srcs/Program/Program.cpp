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

void			Program::start(void) {
	std::cout << "Starting server..." << std::endl;
	while (1)
	{
	}
}

void			Program::stop(void) {
	std::cout << "Stopping server..." << std::endl;
}

const char*		Program::IncorrectConfigFileException::what() const throw()
{
	return "Config file is incorrect";
}
