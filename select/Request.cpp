#include "Request.hpp"

char const* Request::BadRequestException::what() const throw()
{
	return "bad request";
}

char const* Request::VersionNotImplementedException::what() const throw()
{
	return "version not implemented";
}

Request::Request()
{
	return;
}

Request::Request(Request const& src)
{
	*this = src;
	return;
}

Request& Request::operator=(Request const& rhs)
{
	this->content = rhs.content;
	this->is_bad = rhs.is_bad;
	this->error_type = rhs.error_type;
	this->method = rhs.method;
	this->uri = rhs.uri;
	this->http_version = rhs.http_version;
	this->host = rhs.host;
	this->port = rhs.port;
	return *this;
}

std::string Request::receiveContent(int const& request_fd)
{
	static_cast<void>(request_fd);
	return "";
}

std::size_t Request::parseMethod()
{
	if (this->content.compare(0, 3, "GET") == 0)
	{
		this->method = kGet;
		return 3;
	}
	else if (this->content.compare(0, 4, "POST") == 0)
	{
		this->method = kGet;
		return 4;
	}
	else if (this->content.compare(0, 6, "DELETE") == 0)
	{
		this->method = kDelete;
		return 6;
	}
	throw Request::BadRequestException();
}

std::size_t Request::parseUri(std::size_t pos)
{
	if (this->content[pos] != '/')
	{
		throw Request::BadRequestException();
	}
	std::size_t uri_length = this->content.find(' ', pos) - pos;
	if (uri_length + pos == this->content.npos)
	{
		throw Request::BadRequestException();
	}
	this->uri = this->content.substr(pos, uri_length);
	return pos + uri_length;
}

std::size_t Request::parseHttpVersion(std::size_t pos)
{
	if (this->content.compare(pos, 5, "HTTP/") != 0)
	{
		throw Request::BadRequestException();
	}
	pos += 5;
	if (std::isdigit(this->content[pos]) == 0 || this->content[pos + 1] != '.'
			|| std::isdigit(this->content[pos + 2]) == 0)
	{
		throw Request::BadRequestException();
	}
	else if (this->content[pos] != '1' || this->content[pos + 2] > '1')
	{
		throw Request::VersionNotImplementedException();
	}
	this->http_version = std::strtod(this->content.substr(pos, 3).c_str(), NULL);
	return pos + 3;
}

std::size_t Request::parseRequestLine()
{
	std::size_t pos = this->parseMethod();
	if (this->content[pos] != ' ')
	{
		throw Request::BadRequestException();
	}
	pos = this->parseUri(pos + 1);
	if (this->content[pos] != ' ')
	{
		throw Request::BadRequestException();
	}
	pos = this->parseHttpVersion(pos + 1);
	if (this->content.compare(pos, 2, "\r\n") != 0)
	{
		throw Request::BadRequestException();
	}
	return pos + 2;
}

void Request::parseHostFieldValue(std::string const& field_value)
{
	std::size_t colon_pos = field_value.find(':');
	this->host = field_value.substr(0, colon_pos);
	if (colon_pos == field_value.npos)
	{
		this->port = 80;
	}
	else
	{
		this->port = std::atoi(field_value.substr(colon_pos + 1).c_str());
	}
	return;
}

void Request::parseHeaderField(std::string const& header_field)
{
	std::size_t colon_pos = header_field.find(':');
	std::string field_name = header_field.substr(0, colon_pos);
	std::size_t field_value_start_pos = header_field.find_first_not_of(" \t", colon_pos + 1);
	std::size_t field_value_end_pos = header_field.find_last_not_of(" \t");
	if (colon_pos == header_field.npos
			|| field_value_start_pos == header_field.npos)
	{
		throw Request::BadRequestException();
	}
	std::string field_value = header_field.substr(field_value_start_pos,
			field_value_end_pos - field_value_start_pos + 1);
	if (field_name == "Host")
	{
		this->parseHostFieldValue(field_value);
	}
	return;
}

std::size_t Request::parseHeaders(std::size_t pos)
{
	std::size_t crlf_pos = this->content.find("\r\n", pos);
	if (crlf_pos == this->content.npos)
	{
		throw Request::BadRequestException();
	}
	std::string header_field = this->content.substr(pos, crlf_pos - pos);
	while (header_field != "")
	{
		this->parseHeaderField(header_field);
		pos = crlf_pos + 2;
		crlf_pos = this->content.find("\r\n", pos);
		if (crlf_pos == this->content.npos)
		{
			throw Request::BadRequestException();
		}
		header_field = this->content.substr(pos, crlf_pos - pos);
	}
	return crlf_pos + 2;
}

void Request::parseContent()
{
	try
	{
		std::size_t pos = this->parseRequestLine();
		pos = this->parseHeaders(pos);
		this->body = this->content.substr(pos);
	}
	catch (Request::BadRequestException const& e)
	{
		this->is_bad = true;
		this->error_type = kBadRequest;
	}
	catch (Request::VersionNotImplementedException const& e)
	{
		this->is_bad = true;
		this->error_type = kVersionNotImplemented;
	}
	return;
}

Request::Request(std::string const& content) :
	content(content),
	is_bad(false)
{
	this->parseContent();
	return;
}

Request::Request(int const& request_fd) :
	content(""),
	is_bad(false)
{
	this->content = this->receiveContent(request_fd);
	this->parseContent();
	return;
}

Request::~Request()
{
	return;
}

std::string const& Request::getContent() const
{
	return this->content;
}

bool const& Request::getIs_bad() const
{
	return this->is_bad;
}

HttpErrorType const& Request::getError_type() const
{
	return this->error_type;
}

HttpMethod const& Request::getMethod() const
{
	return this->method;
}

std::string const& Request::getUri() const
{
	return this->uri;
}

double const& Request::getHttp_version() const
{
	return this->http_version;
}

std::string const& Request::getHost() const
{
	return this->host;
}

int const& Request::getPort() const
{
	return this->port;
}

std::string const& Request::getBody() const
{
	return this->body;
}

std::ostream& operator<<(std::ostream& os, Request const& req)
{
	if (req.getIs_bad() == true)
	{
		os << "error_type : ";
		if (req.getError_type() == kBadRequest)
		{
			os << "bad request" << std::endl;
		}
		else
		{
			os << "version not implemented" << std::endl;
		}
		return os;
	}
	os << "method : ";
	if (req.getMethod() == kGet) 
	{
		os << "GET" << std::endl;
	}
	else if (req.getMethod() == kPost)
	{
		os << "POST" << std::endl;
	}
	else
	{
		os << "DELETE" << std::endl;
	}
	os << "uri : " << req.getUri() << std::endl;
	os << "http_version : " << req.getHttp_version() << std::endl;
	os << "host : " << req.getHost() << std::endl;
	os << "port : " << req.getPort() << std::endl;
	return os;
}
