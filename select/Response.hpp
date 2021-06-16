#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Request.hpp"

class Response {
private:
	double http_version_;
	int status_code_;
	std::string reason_phrase_;
	static std::string const server_;
	std::string content_type_;
	std::size_t content_length_;
	std::string body_;

	Response();
	Response(Response const& src);

	Response& operator=(Response const& rhs);

	std::string readUri(std::string const& uri);

public:
	Response(Request const& request);
	~Response();

	std::string toString() const;
};

#endif
