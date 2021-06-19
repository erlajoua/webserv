#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <sstream>
# include <fstream>
# include <iostream>
# include <sys/stat.h>
# include <unistd.h>

# include "./Request.hpp"

std::string	getAllFile(std::string filename);

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

	Response();
	Response(Response const& src);

	Response& operator =(Response const& rhs);

	void		createResponse(Request const &request);
	void		setStatusCode(Request const &request);
	void		setBody(Request const &request);
	void		setReasonPhrase(void);
	void		setContentLength(void);
	void		setContentType(void);
	
	std::string	getErrorPage(void);

	std::string readUri(std::string const& uri);

public:
	Response(Request const& request);
	~Response();

	std::string toString() const;
};

#endif
