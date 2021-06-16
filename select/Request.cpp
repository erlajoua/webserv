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
	content_ = rhs.content_;
	method_ = rhs.method_;
	uri_ = rhs.uri_;
	http_version_ = rhs.http_version_;
	host_ = rhs.host_;
	port_ = rhs.port_;
	return *this;
}

std::string Request::receiveContent(int const& request_fd)
{
	static_cast<void>(request_fd);
	return "";
}

std::size_t Request::parseMethod()
{
	if (content_.compare(0, 3, "GET") == 0)
	{
		method_ = kGet;
		return 3;
	}
	else if (content_.compare(0, 4, "POST") == 0)
	{
		method_ = kGet;
		return 4;
	}
	else if (content_.compare(0, 6, "DELETE") == 0)
	{
		method_ = kDelete;
		return 6;
	}
	throw Request::BadRequestException();
}

std::size_t Request::parseUri(std::size_t pos)
{
	if (content_[pos] != '/')
	{
		throw Request::BadRequestException();
	}
	std::size_t uri_length = content_.find(' ', pos) - pos;
	if (uri_length + pos == content_.npos)
	{
		throw Request::BadRequestException();
	}
	uri_ = content_.substr(pos, uri_length);
	return pos + uri_length;
}

std::size_t Request::parseHttpVersion(std::size_t pos)
{
	if (content_.compare(pos, 5, "HTTP/") != 0)
	{
		throw Request::BadRequestException();
	}
	pos += 5;
	if (std::isdigit(content_[pos]) == 0 || content_[pos + 1] != '.'
			|| std::isdigit(content_[pos + 2]) == 0)
	{
		throw Request::BadRequestException();
	}
	else if (content_[pos] != '1' || content_[pos + 2] > '1')
	{
		throw Request::VersionNotImplementedException();
	}
	http_version_ = std::strtod(content_.substr(pos, 3).c_str(), NULL);
	return pos + 3;
}

std::size_t Request::parseRequestLine()
{
	std::size_t pos = this->parseMethod();
	if (content_[pos] != ' ')
	{
		throw Request::BadRequestException();
	}
	pos = this->parseUri(pos + 1);
	if (content_[pos] != ' ')
	{
		throw Request::BadRequestException();
	}
	pos = this->parseHttpVersion(pos + 1);
	if (content_.compare(pos, 2, "\r\n") != 0)
	{
		throw Request::BadRequestException();
	}
	return pos + 2;
}

void Request::parseHostFieldValue(std::string const& field_value)
{
	std::size_t colon_pos = field_value.find(':');
	host_ = field_value.substr(0, colon_pos);
	if (colon_pos == field_value.npos)
	{
		port_ = 80;
	}
	else
	{
		port_ = std::atoi(field_value.substr(colon_pos + 1).c_str());
	}
	return;
}

void Request::parseHeaderField(std::string const& header_field)
{
	std::size_t colon_pos = header_field.find(':');
	std::string field_name = header_field.substr(0, colon_pos);
	std::size_t field_value_start_pos = header_field.find_first_not_of(" \t", colon_pos + 1);
	std::size_t field_value_end_pos = header_field.find_first_of(" \t", field_value_start_pos);
	if (colon_pos == header_field.npos
			|| field_value_start_pos == header_field.npos
			|| (field_value_end_pos != header_field.npos
				&& header_field.find_first_not_of(" \t",
					field_value_end_pos + 1) != header_field.npos))
	{
		throw Request::BadRequestException();
	}
	std::string field_value = header_field.substr(field_value_start_pos,
			field_value_end_pos - field_value_start_pos);
	if (field_name == "Host")
	{
		this->parseHostFieldValue(field_value);
	}
	return;
}

std::size_t Request::parseHeaders(std::size_t pos)
{
	std::size_t crlf_pos = content_.find("\r\n", pos);
	if (crlf_pos == content_.npos)
	{
		throw Request::BadRequestException();
	}
	std::string header_field = content_.substr(pos, crlf_pos - pos);
	while (header_field != "")
	{
		this->parseHeaderField(header_field);
		pos = crlf_pos + 2;
		crlf_pos = content_.find("\r\n", pos);
		if (crlf_pos == content_.npos)
		{
			throw Request::BadRequestException();
		}
		header_field = content_.substr(pos, crlf_pos - pos);
	}
	return crlf_pos + 2;
}

void Request::parseContent()
{
	try
	{
		std::size_t pos = this->parseRequestLine();
		pos = this->parseHeaders(pos);
		body_ = content_.substr(pos);
	}
	catch (Request::BadRequestException const& e)
	{
		is_bad_ = true;
		error_type_ = kBadRequest;
	}
	catch (Request::VersionNotImplementedException const& e)
	{
		is_bad_ = true;
		error_type_ = kVersionNotImplemented;
	}
	return;
}

Request::Request(std::string const& content) :
	content_(content),
	is_bad_(false)
{
	this->parseContent();
	return;
}

Request::Request(int const& request_fd) :
	content_(""),
	is_bad_(false)
{
	content_ = this->receiveContent(request_fd);
	this->parseContent();
	return;
}

Request::~Request()
{
	return;
}

std::string const& Request::content() const
{
	return content_;
}

bool const& Request::is_bad() const
{
	return is_bad_;
}

HttpErrorType const& Request::error_type() const
{
	return error_type_;
}

HttpMethod const& Request::method() const
{
	return method_;
}

std::string const& Request::uri() const
{
	return uri_;
}

double const& Request::http_version() const
{
	return http_version_;
}

std::string const& Request::host() const
{
	return host_;
}

int const& Request::port() const
{
	return port_;
}

std::string const& Request::body() const
{
	return body_;
}

std::ostream& operator<<(std::ostream& os, Request const& req)
{
	if (req.is_bad() == true)
	{
		os << "error_type : ";
		if (req.error_type() == kBadRequest)
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
	if (req.method() == kGet) 
	{
		os << "GET" << std::endl;
	}
	else if (req.method() == kPost)
	{
		os << "POST" << std::endl;
	}
	else
	{
		os << "DELETE" << std::endl;
	}
	os << "uri : " << req.uri() << std::endl;
	os << "http_version : " << req.http_version() << std::endl;
	os << "host : " << req.host() << std::endl;
	os << "port : " << req.port() << std::endl;
	return os;
}
