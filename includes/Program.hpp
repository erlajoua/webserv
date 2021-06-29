/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Program.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 14:46:51 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/29 14:02:15 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROGRAM_HPP
# define PROGRAM_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <vector>
# include <map>
# include <stdexcept>
# include <signal.h>
# include <iterator>
# include <unistd.h>
# include <sys/select.h>
# include <fcntl.h>
# include <errno.h>

# include "Server.hpp"

class Server;

enum EnvpIndexes {
	kScriptFilename = 0,
	kRedirectStatus,
	kRequestMethod,
	kQueryString,
	kContentType,
	kContentLength,
	kHttpCookie
};

class Program {
private:
	// ATTRIBUTES
	std::vector<Server>	servers;
	char				**envp;
	fd_set				readfds;
	fd_set				writefds;
	bool				is_running;

	std::map<int, std::string>	pending_requests_content;

	// UNUSED NORMALIZED FUNCTIONS
	Program(Program const &p);
	Program		&operator=(Program const &p);

	// PRIVATE HELPERS
	bool		isServConfig(std::string const &line) const;
	bool 		isLocationConfig(std::string const &line) const;
	bool		isFieldSingle(std::string const &line, std::string const &field) const;
	bool		isFieldMultiple(std::string const &line, std::string const &field) const;
	bool		isField3(std::string const &line, std::string const &field) const;
	bool 		isServField(std::string const &line) const;
	bool 		isLocationField(std::string const &line) const;
	bool		isClosingBracket(std::string const &line) const;
	bool		isEmptyLine(std::string const &line) const;

	void		checkNoServer(std::vector<std::string> lines);
	void		checkInvalidInstruction(std::vector<std::string> lines);
	void		checkInvalidDeclaration(std::vector<std::string> lines);
	void		checkErrorConfig(std::vector<std::string> lines);

	Server		setServField(Server s, std::string const &field);
	Location	setLocationField(Server s, Location r, std::string const &field);
	void		parseValue(std::vector<std::string> lines);

	void		checkMinimumSetup(void);
	bool		hasAlreadyDefaultServer(std::vector<Server>::iterator iter);
	void		setDefaultServer(void);
	void		setDefaultRoutes(void);

	void		setupEnvp(char **envp);

	void		acceptNewServerConnection(int server_socket);
	void		handleRequest(int client_socket);
	void		httpServerIO(void);

public:
	// CONSTRUCTOR & DESTRUCTOR
	Program(void);
	~Program(void);

	// MEMBER FUNCTIONS
	void		parseConfig(std::string path);
	void 		printParsing(void);
	void		setup(char **envp);
	void		start(void);
	void		stop(void);

	void		setIsRunning(bool is_running_value);

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
	class 		InvalidLocationFieldException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		NoPortSetupException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		NoHostSetupException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		NoRootSetupException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		NoCgiBinException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		SelectException: public std::exception {
		virtual const char* what() const throw();
	};
};

#endif
