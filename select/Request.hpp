#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <cstdlib>

#include <string>
#include <sstream>
#include <iostream>
#include <exception>

enum HttpErrorType {
	kBadRequest,
	kVersionNotImplemented
};

enum HttpMethod {
	kGet,
	kPost,
	kDelete
};

class Request {
private:
	class BadRequestException : public std::exception {
	public:
		virtual char const* what() const throw();
	};

	class VersionNotImplementedException : public std::exception {
	public:
		virtual char const* what() const throw();
	};

	std::string content_;

	bool is_bad_;
	HttpErrorType error_type_;

	HttpMethod method_;
	std::string uri_;
	double http_version_;

	std::string host_;
	int port_;

	std::string body_;

	Request();
	Request(Request const& src);

	Request& operator=(Request const& rhs);

	std::string receiveContent(int const& request_fd);

	std::size_t parseMethod();
	std::size_t parseUri(std::size_t pos);
	std::size_t parseHttpVersion(std::size_t pos);
	std::size_t parseRequestLine();
	void parseHostFieldValue(std::string const& field_value);
	void parseHeaderField(std::string const& header_field);
	std::size_t parseHeaders(std::size_t pos);
	void parseContent();

public:
	Request(std::string const& content);
	Request(int const& request_fd);
	~Request();

	std::string const& content() const;
	bool const& is_bad() const;
	HttpErrorType const& error_type() const;
	HttpMethod const& method() const;
	std::string const& uri() const;
	double const& http_version() const;
	std::string const& host() const;
	int const& port() const;
	std::string const& body() const;
};

std::ostream& operator<<(std::ostream& os, Request const& req);

#endif
