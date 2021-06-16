/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 14:29:26 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/14 16:54:14 by nessayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.hpp"

Program			program;

void	quit(int arg)
{
	(void)arg;
	std::cout << "\b\b  " << std::endl;
	program.stop();
	std::cout << BOLDGREEN << "The user has stopped the program. 👋👋👋" << RESET << std::endl;
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
	}
	catch (std::exception &e)
	{
		std::cerr << BOLDRED << std::endl << "=> " << e.what() << RESET << std::endl << std::endl;	
		return (0);
	}
	signal(SIGINT, quit);
	program.printSetup();
	program.start();
	return (0);
}
