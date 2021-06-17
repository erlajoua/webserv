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
	bool		isField3(std::string const &line, std::string const &field) const;
	bool 		isServField(std::string const &line) const;
	bool 		isRouteField(std::string const &line) const;
	bool		isClosingBracket(std::string const &line) const;
	bool		isEmptyLine(std::string const &line) const;

	void		checkNoServer(std::vector<std::string> lines);
	void		checkInvalidInstruction(std::vector<std::string> lines);
	void		checkInvalidDeclaration(std::vector<std::string> lines);
	void		checkErrorConfig(std::vector<std::string> lines);

	Server		setServField(Server s, std::string const &field);
	Route		setRouteField(Server s, Route r, std::string const &field);
	void		parseValue(std::vector<std::string> lines);

	void		checkMinimumSetup(void);

public:
	// CONSTRUCTOR & DESTRUCTOR
	Program(void);
	~Program(void);

	// MEMBER FUNCTIONS
	void		parseConfig(std::string path);
	void 		printParsing(void);
	void		start(void);
	void		stop(void);

	// EXCEPTIONS
	class 		NoServerException: public std::exception {
		virtual const char* what() const throw();
	};

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

	class 		RootNoneException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		SameMethodException: public std::exception {
		virtual const char* what() const throw();
	};

	class 		NoPortSetupException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		NoHostSetupException: public std::exception {
		virtual const char* what() const throw();
	};
};

#endif
