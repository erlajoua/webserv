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
	std::cout << "The user has stopped the program." << std::endl;
	exit(0);
}

int		main(int argc, char **argv)
{
	std::string		path;
	std::ifstream	file;

	if (argc == 1)
		path = "./config/default.conf";
	else if (argc == 2)
		path = argv[1];
	else
	{
		std::cerr << "0 or 1 argument (= path to config file)." << std::endl;
		return (0);
	}

	file.open(path);
	if (!file.is_open())
	{
		std::cerr << "Config file at path " << path << " can't be opened" << std::endl;
		return (0);
	}
	file.close();

	try
	{	
		program.parseConfig(path);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (0);
	}
	signal(SIGINT, quit);
	program.start();
	return (0);
}
