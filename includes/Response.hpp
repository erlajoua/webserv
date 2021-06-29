#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <cstring>
# include <string>
# include <sstream>
# include <fstream>
# include <iostream>
# include <sys/stat.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

# include "Program.hpp"
# include "Server.hpp"
# include "Request.hpp"
# include "AutoIndex.hpp"

std::string	getAllFile(std::string filename);

class Server;

# define DEFAULT_PATH_ERROR "./www/error_pages/"

class Response
{
private:
	double						http_version;
	int 						status_code;
	std::string 				reason_phrase;
	static std::string const	server;
	std::vector<std::string>	set_cookie_values;
	std::string 				content_type;
	std::size_t 				content_length;
	std::string 				body;
	bool						is_autoindex;

	std::string					full_path;
	std::string					redirection_path;

	std::string 				cgi_content_type;

	Response();
	Response(Response const& src);

	Response& operator =(Response const& rhs);

	void		createResponse(char **envp, Request &request, Server &server);
	void		setStatusCode(Request &request, Server &server);
	void		setBody(char **envp, std::string const &uri,
			Request const &request, Server &server);
	void		setReasonPhrase(void);
	void		setContentLength(void);
	void		setContentType(void);

	void		setCgiEnvVar(char **envp, Request const &request,
			std::string const& script_filename);
	void		deleteCgiEnvVar(char **envp);
	void		closeCgiPipes(int pipes_fds[2][2]);
	void		setCgiPipes(int pipes_fds[2][2]);
	void		dupCgiProcessStreams(std::string const &request_body,
			int pipes_fds[2][2]);
	void		cgiProcess(char **envp, std::string const &request_body,
			std::string const &cgi_bin, int pipes_fds[2][2]);
	void		callCgi(char **envp, Request const &request,
			std::string const& cgi_bin, int pipes_fds[2][2]);
	std::string readCgiOutput(int pipes_fds[2][2]);
	std::size_t	parseCgiOutputHeaders(std::string const &output);
	std::string getCgiOutputBody(char **envp, Request const &request,
			std::string const& script_filename, std::string const& cgi_bin);

	void		handleFolderPath(Request &request, Server &server);
	void		handleFilePath(Request &request);
	void		handleAutoIndex(Location &location);
	std::string	findCustomErrorPage(Server &server, int status_code);
	std::string buildStandardErrorPage(int status_code);

	Location&	getLocation(Server &server, std::string uri);
	int			checkMethodsAllowed(Server &server, Request &request);



public:
	Response(char **envp, Request &request, Server &server);
	~Response();

	std::string toString() const;

	class 		defaultLocationNotFound: public std::exception
	{
		virtual const char* what() const throw();
	};
	class 		IndexLocationNotFile: public std::exception
	{
		virtual const char* what() const throw();
	};
};

#endif
