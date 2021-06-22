/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 14:29:26 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/22 18:36:59 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Program			program;

std::string	getAllFile(std::string filename)
{
	std::ifstream input;

	input.open(filename.c_str());

	std::string file_content;
	std::string buf;

	while (input.bad() == false && input.eof() == false)
	{
		std::getline(input, buf);
		file_content += buf;
		if (input.eof() == false)
			file_content += "\n";
	}
	return (file_content);
}

void	quit(int arg)
{
	(void)arg;
	std::cout << "\b\b  " << std::endl;
	program.stop();
	std::cout << std::endl << BOLDGREEN << "The user has stopped the program. 👋👋👋" << RESET << std::endl;
	exit(0);
}

int		main(int argc, char **argv)
{
	std::string		path;

	if (argc == 1)
		path = "./config/default.conf";
	else if (argc == 2)
		path = argv[1];
	else
	{
		std::cerr << BOLDRED << "0 or 1 argument (= path to config file)." << RESET << std::endl;
		return (0);
	}

	std::ifstream	file(path.c_str());
	if (!file.is_open())
	{
		std::cerr << BOLDRED << "Config file at path " << path << " can't be opened." << RESET << std::endl;
		return (0);
	}
	file.close();

	try
	{	
		program.parseConfig(path);
		program.printParsing();
		program.setup();
	}
	catch (std::exception &e)
	{
		std::cerr << BOLDRED << std::endl << "=> " << e.what() << RESET << std::endl << std::endl;	
		return (0);
	}
	signal(SIGINT, quit);
	program.start();
	//AutoIndex a("./www", "/even_pages");
	//a.buildAutoIndex();
	//std::cout << CYAN << a.getPageContent() << RESET << std::endl;
	return (0);
}
