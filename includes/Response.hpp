#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <sstream>
# include <fstream>
# include <iostream>
# include <sys/stat.h>
# include <unistd.h>

# include "./Server.hpp"
# include "./Request.hpp"

std::string	getAllFile(std::string filename);

class Server;

class Response
{
private:
	double						http_version;
	int 						status_code;
	std::string 				reason_phrase;
	static std::string const	server;
	std::string 				content_type;
	std::size_t 				content_length;
	std::string 				body;
	bool						auto_index;

	std::string					full_path;

	Response();
	Response(Response const& src);

	Response& operator =(Response const& rhs);

	void		createResponse(Request &request, Server &server);
	void		setStatusCode(Request &request, Server &server);
	void		setBody(Request const &request, Server &server);
	void		setReasonPhrase(void);
	void		setContentLength(void);
	void		setContentType(void);

	std::string	getErrorPage(void);

	std::string readUri(std::string const& uri);
	void		handleUnknownPath(Request const &request, Server &server);
	void		handleFolderPath(Request &request, Server &server);
	void		handleFilePath(Request &request, Server &server);

	int			getPositionLastChar(char *str, char c) const;
	Location&	getLocation(Server &server, std::string uri);


public:
	Response(Request & request, Server &server);
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
