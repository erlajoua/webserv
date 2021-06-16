/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Program.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 14:46:51 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/16 06:55:27 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROGRAM_HPP
# define PROGRAM_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <vector>
# include <stdexcept>
# include <signal.h>
# include <iterator>

# include "./Server.hpp"

class Program {
private:
	// ATTRIBUTES
	std::vector<Server> servers;

	// UNUSED NORMALIZED FUNCTIONS
	Program(Program const &p);
	Program		&operator=(Program const &p);

	// HELPERS
	int			checkErrorConfig(void);
	bool		isServConfig(std::string const &line) const;
	bool 		isRouteConfig(std::string const &line) const;
	bool		isField(std::string const &line, std::string const &field) const;
	bool		isClosingBracket(std::string const &line) const;
	bool		isEmptyLine(std::string const &line) const;

	void		revealLines(std::vector<std::string> lines);
	void		parseServers(std::vector<std::string> lines);

public:
	// CONSTRUCTOR & DESTRUCTOR
	Program(void);
	~Program(void);

	// MEMBER FUNCTIONS
	void		parseConfig(std::string path);
	void		start(void);
	void		stop(void);

	// EXCEPTIONS
	class 		IncorrectConfigFileException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		MissingCurlyBracketException: public std::exception {
		virtual const char* what() const throw();
	};
};

#endif
