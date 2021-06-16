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
# include <unistd.h>

# include "./Server.hpp"
# include "./Route.hpp"

# define RESET   		"\033[0m"
# define BLACK   		"\033[30m"      		/* Black */
# define RED     		"\033[31m"      		/* Red */
# define GREEN   		"\033[32m"      		/* Green */
# define YELLOW  		"\033[33m"      		/* Yellow */
# define BLUE    		"\033[34m"      		/* Blue */
# define MAGENTA 		"\033[35m"      		/* Magenta */
# define CYAN    		"\033[36m"      		/* Cyan */
# define WHITE   		"\033[37m"      		/* White */
# define BOLDBLACK		"\033[1m\033[30m"      	/* Bold Black */
# define BOLDRED		"\033[1m\033[31m"      	/* Bold Red */
# define BOLDGREEN		"\033[1m\033[32m"      	/* Bold Green */
# define BOLDYELLOW		"\033[1m\033[33m"      	/* Bold Yellow */
# define BOLDBLUE		"\033[1m\033[34m"      	/* Bold Blue */
# define BOLDMAGENTA	"\033[1m\033[35m"      	/* Bold Magenta */
# define BOLDCYAN		"\033[1m\033[36m"      	/* Bold Cyan */
# define BOLDWHITE		"\033[1m\033[37m"      	/* Bold White */

class Program {
private:
	// ATTRIBUTES
	std::vector<Server> servers;

	// UNUSED NORMALIZED FUNCTIONS
	Program(Program const &p);
	Program		&operator=(Program const &p);

	// PRIVATE HELPERS
	bool		isServConfig(std::string const &line) const;
	bool 		isRouteConfig(std::string const &line) const;
	bool		isFieldSingle(std::string const &line, std::string const &field) const;
	bool		isFieldMultiple(std::string const &line, std::string const &field) const;
	bool 		isServField(std::string const &line) const;
	bool 		isRouteField(std::string const &line) const;
	bool		isClosingBracket(std::string const &line) const;
	bool		isEmptyLine(std::string const &line) const;

	void		checkInvalidInstruction(std::vector<std::string> lines);
	void		checkInvalidDeclaration(std::vector<std::string> lines);
	void		checkErrorConfig(std::vector<std::string> lines);

	void		setServField(Server s, std::string const &field);
	void		setRouteField(Route r, std::string const &field);
	void		parseValue(std::vector<std::string> lines);

public:
	// CONSTRUCTOR & DESTRUCTOR
	Program(void);
	~Program(void);

	// MEMBER FUNCTIONS
	void		parseConfig(std::string path);
	void		start(void);
	void		stop(void);

	// EXCEPTIONS
	class 		InvalidInstructionException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		ServerFirstException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		InvalidServerFieldException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		InvalidRouteFieldException: public std::exception {
		virtual const char* what() const throw();
	};

	class 		InvalidPortException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		InvalidHostException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		InvalidServerNameException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		InvalidRootException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		InvalidErrorsException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		InvalidClientBodySizeException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		InvalidUploadDirException: public std::exception {
		virtual const char* what() const throw();
	};

	class 		InvalidPathException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		InvalidMethodsException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		InvalidRedirectionException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		InvalidAutoindexException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		InvalidCgiExtensionException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		InvalidCgiBinDirException: public std::exception {
		virtual const char* what() const throw();
	};
};

#endif
