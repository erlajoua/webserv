/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 13:32:01 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/25 07:54:33 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <vector>
# include <sstream>
# include <iostream>
# include <cstdlib>
# include <arpa/inet.h>
# include <sys/stat.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <iterator> 
# include <string.h>

# include "Location.hpp"
# include "Request.hpp"
# include "Response.hpp"

# define NB_CLIENT_MAX 500

class Server
{
private:
	// ATTRIBUTES
	bool						default_server;
	int 						port; 
	std::string 				host;
	std::string 				server_name;
	std::string					root;
	std::vector<std::string>	errors;
	int 						client_body_size;
	std::vector<Location> 		locations;

    sockaddr_in         		addr;
    int                 		server_socket;

public:
	// CONSTRUCTOR & DESTRUCTOR
	Server(void);
	Server(Server const &s);
	~Server(void);

	// MEMBER FUNCTIONS
	void	handleRequest(int client_socket, std::string const &request_content,
			Request &request, char **envp);
	int							acceptNewConnection() const;
	void						setup(void);
	int							hasLocation(std::string uri) const;

	// GETTERS
	bool							getDefaultServer(void) const;
	int								getPort(void) const;
	std::string						getHost(void) const;
	std::string						getServerName(void) const;
	std::string						getRoot(void) const;
	std::vector<std::string> const &getErrors(void) const;
	size_t							getClientBodySize(void) const;
	std::vector<Location>			*getLocations(void);

	int								getServerSocket(void) const;

	// SETTERS
	void						setDefaultServer(void);
	void						setPort(std::string const &field);
	void						setHost(std::string const &field);
	void						setServerName(std::string const &field);
	void						setRoot(std::string const &field);
	void						setErrors(std::string const &root, std::string const &field);
	void						setClientBodySize(std::string const &field);

	// OPERATOR
	Server						&operator=(Server const &s);

	// EXCEPTIONS
	class 		SocketInitializationException: public std::exception {
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
	class 		BindException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		ListenException: public std::exception {
		virtual const char* what() const throw();
	};
	class 		AcceptNewConnectionException: public std::exception {
		virtual const char* what() const throw();
	};
};

#endif
