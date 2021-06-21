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

	void		createResponse(Request const &request, Server &server);
	void		setStatusCode(Request const &request, Server &server);
	void		setBody(Request const &request, Server &server);
	void		setReasonPhrase(void);
	void		setContentLength(void);
	void		setContentType(void);

	std::string	getErrorPage(void);

	std::string readUri(std::string const& uri);
	void		handleUnknownPath(Request const &request, Server &server);
	void		handleFolderPath(Request const &request, Server &server);
	void		handleFilePath(Request const &request, Server &server);

public:
	Response(Request const& request, Server &server);
	~Response();

	std::string toString() const;
};

#endif
