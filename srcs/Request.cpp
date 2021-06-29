/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 12:27:10 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/29 13:59:27 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// constructors

Request::Request(std::string const &content): content(content), is_finished(true),
	is_bad(false), connection(kClose), content_length(0) {
	this->parseContent();
}

Request::Request(Request const &r) {
	*this = r;
}

// destructors

Request::~Request(void) {
}

// PRIVATE HELPERS

std::size_t 		Request::parseMethod(void)
{
	if (this->content.compare(0, 3, "GET") == 0)
	{
		this->method = kGet;
		return (3);
	}
	else if (this->content.compare(0, 4, "POST") == 0)
	{
		this->method = kPost;
		return (4);
	}
	else if (this->content.compare(0, 6, "DELETE") == 0)
	{
		this->method = kDelete;
		return (6);
	}
	throw Request::BadRequestException();
}

void 				Request::decodeUri(void) {
	std::size_t pos = this->uri.find('%');
	while (pos != this->uri.npos)
	{
		if (std::isxdigit(this->uri[pos + 1]) == 0
				|| std::isxdigit(this->uri[pos + 2]) == 0)
		{
			throw Request::BadRequestException();
		}
		this->uri.replace(pos, 3, 1, std::strtol(this->uri.substr(
						pos + 1, 2).c_str(), NULL, 16));
		pos = this->uri.find('%', pos + 1);
	}
}

std::size_t 		Request::parseUri(std::size_t pos) {
	std::size_t uri_length = this->content.find_first_of("? ", pos) - pos;
	if (this->content[pos] != '/'
			|| uri_length + pos == this->content.npos)
	{
		throw Request::BadRequestException();
	}
	this->uri = this->content.substr(pos, uri_length);
	this->decodeUri();
	return (pos + uri_length);
}

std::size_t 		Request::parseQueryString(std::size_t pos) {
	this->query_string = "";
	if (this->content[pos] == '?')
	{
		pos++;
		std::size_t query_string_length = this->content.find(' ', pos) - pos;
		if (query_string_length + pos == this->content.npos)
		{
			throw Request::BadRequestException();
		}
		this->query_string = this->content.substr(pos, query_string_length);
		pos += query_string_length;
	}
	return (pos);
}

std::size_t 		Request::parseHttpVersion(std::size_t pos) {
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
	return (pos + 3);
}

std::size_t 		Request::parseRequestLine(void) {
	std::size_t pos = this->parseMethod();
	if (this->content[pos] != ' ')
	{
		throw Request::BadRequestException();
	}
	pos = this->parseUri(pos + 1);
	pos = this->parseQueryString(pos);
	if (this->content[pos] != ' ')
	{
		throw Request::BadRequestException();
	}
	pos = this->parseHttpVersion(pos + 1);
	if (this->content.compare(pos, 2, "\r\n") != 0)
	{
		throw Request::BadRequestException();
	}
	return (pos + 2);
}

void 				Request::parseHostFieldValue(std::string const& field_value) {
	std::size_t colon_pos = field_value.find(':');
	this->host = field_value.substr(0, colon_pos);
	if (colon_pos == field_value.npos)
		this->port = 80;
	else
		this->port = std::atoi(field_value.substr(colon_pos + 1).c_str());
}

void 				Request::parseConnectionFieldValue(std::string const& field_value) {
	if (field_value == "keep-alive")
	{
		this->connection = kKeepAlive;
	}
}

void 				Request::parseContentLengthFieldValue(std::string const& field_value) {
	this->content_length = std::atoi(field_value.c_str());
}

void 				Request::parseContentTypeFieldValue(std::string const& field_value) {
	this->content_type = field_value;
}

void 				Request::parseCookieFieldValue(std::string const& field_value) {
	this->cookie = field_value;
}

void 				Request::parseHeaderField(std::string const& header_field) {
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
	else if (field_name == "Connection")
	{
		this->parseConnectionFieldValue(field_value);
	}
	else if (field_name == "Content-Length")
	{
		this->parseContentLengthFieldValue(field_value);
	}
	else if (field_name == "Content-Type")
	{
		this->parseContentTypeFieldValue(field_value);
	}
	else if (field_name == "Cookie")
	{
		this->parseCookieFieldValue(field_value);
	}
}

std::size_t 		Request::parseHeaders(std::size_t pos) {
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
	return (crlf_pos + 2);
}

void 				Request::parseContent(void) {
	try
	{
		if (this->content.find("\n\r\n") == this->content.npos)
		{
			this->is_finished = false;
			return;
		}
		std::size_t pos = this->parseRequestLine();
		pos = this->parseHeaders(pos);
		this->body = this->content.substr(pos, this->content_length);
		if (this->body.length() < this->content_length)
		{
			this->is_finished = false;
		}
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
}

// OPERATOR

Request				&Request::operator=(Request const &rhs) {
	this->content = rhs.content;
	this->is_bad = rhs.is_bad;
	this->error_type = rhs.error_type;
	this->method = rhs.method;
	this->uri = rhs.uri;
	this->http_version = rhs.http_version;
	this->host = rhs.host;
	this->port = rhs.port;
	return (*this);
}

// GETTERS

std::string const	&Request::getContent(void) const
{
	return (this->content);
}

bool const			&Request::getIsFinished(void) const
{
	return (this->is_finished);
}

bool const			&Request::getIsBad(void) const
{
	return (this->is_bad);
}

HttpErrorType const	&Request::getErrorType(void) const
{
	return (this->error_type);
}

HttpMethod const 	&Request::getMethod(void) const
{
	return (this->method);
}

std::string const 	&Request::getUri(void) const
{
	return (this->uri);
}

std::string const 	&Request::getQueryString(void) const
{
	return (this->query_string);
}

double const		&Request::getHttpVersion(void) const
{
	return (this->http_version);
}

std::string const 	&Request::getHost(void) const
{
	return (this->host);
}

int const			&Request::getPort(void) const
{
	return (this->port);
}

ConnectionDirective const	&Request::getConnection(void) const
{
	return (this->connection);
}

std::size_t const 	&Request::getContentLength(void) const
{
	return (this->content_length);
}

std::string const 	&Request::getContentType(void) const
{
	return (this->content_type);
}

std::string const 	&Request::getCookie(void) const
{
	return (this->cookie);
}

std::string const	&Request::getBody(void) const
{
	return (this->body);
}

// SETTERS

void		Request::setUri(std::string uri)
{
	this->uri = uri;
}

void		Request::setConnection(ConnectionDirective const& connection)
{
	this->connection = connection;
}

// EXCEPTIONS

char const* Request::BadRequestException::what() const throw()
{
	return ("Request error: bad request.");
}

char const* Request::VersionNotImplementedException::what() const throw()
{
	return ("Request error: version not implemented.");
}

// OVERLOAD

std::ostream &operator<<(std::ostream &os, Request const &req)
{
	if (req.getIsBad() == true)
	{
		os << "error_type : ";
		if (req.getErrorType() == kBadRequest)
			os << "bad request" << std::endl;
		else
			os << "version not implemented" << std::endl;
		return (os);
	}
	os << "method : ";
	if (req.getMethod() == kGet) 
		os << "GET" << std::endl;
	else if (req.getMethod() == kPost)
		os << "POST" << std::endl;
	else
		os << "DELETE" << std::endl;
	os << "uri : \"" << req.getUri() << "\"" << std::endl;
	os << "query_string : \"" << req.getQueryString() << "\"" << std::endl;
	os << "http_version : " << req.getHttpVersion() << std::endl;
	os << "host : \"" << req.getHost() << "\"" << std::endl;
	os << "port : " << req.getPort() << std::endl;
	os << "content_type : " << req.getContentType() << std::endl;
	os << "content_length : " << req.getContentLength() << std::endl;
	os << "cookie : " << req.getCookie() << std::endl;

	if (req.getMethod() == kPost)
		os << "body : \"" << req.getBody() << "\"" << std::endl;
	
	return (os);
}
