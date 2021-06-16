#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Request.hpp"

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

	std::string readUri(std::string const& uri);

public:
	Response(Request const& request);
	~Response();

	std::string toString() const;
};

#endif
