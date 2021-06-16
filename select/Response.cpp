#include "Response.hpp"

std::string const Response::server = "webserver/1.0";

//constructors

Response::Response()
{
}

Response::Response(Response const& src)
{
	*this = src;
}

//destructors

Response::~Response()
{
}

//methods

std::string Response::readUri(std::string const& uri)
{
	std::ifstream ifs(("." + uri).c_str());

	if (ifs.is_open() == false)
	{
		return ("open error");
	}

	std::string file_content;
	std::string buf;

	while (ifs.bad() == false && ifs.eof() == false)
	{
		std::getline(ifs, buf);
		file_content += buf;
		if (ifs.eof() == false)
			file_content += "\n";
	}
	return (file_content);
}

Response::Response(Request const& request)
{
	this->http_version = request.getHttp_version();
	this->body = readUri(request.getUri());
	this->content_length = this->body.length();
	this->status_code = 200;
	this->reason_phrase = "OK";
	this->content_type = "text/html";
}

std::string Response::toString() const
{
	std::stringstream ss;
	ss << "HTTP/" << this->http_version << " " << this->status_code << " "
		<< this->reason_phrase << "\r\nServer: " << this->server
		<< "\r\nContent-Type: " << this->content_type
		<< "\r\nContent-Length: " << this->content_length << "\r\n\r\n" << this->body;
	return (ss.str());
}

//overloads

Response& Response::operator=(Response const& rhs)
{
	this->http_version = rhs.http_version;
	this->status_code = rhs.status_code;
	this->reason_phrase = rhs.reason_phrase;
	this->content_type = rhs.content_type;
	this->content_length = rhs.content_length;
	this->body = rhs.body;
	return (*this);
}