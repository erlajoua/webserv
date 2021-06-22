/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 12:26:29 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/21 10:20:32 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <cstdlib>

enum HttpErrorType
{
	kBadRequest,
	kVersionNotImplemented
};

enum HttpMethod
{
	kGet,
	kPost,
	kDelete
};

class Request
{
private:
	// ATTRIBUTES
	std::string		content;
	bool			is_bad;
	HttpErrorType	error_type;
	HttpMethod		method;
	std::string		uri;
	std::string		query_string;
	double			http_version;
	std::string		host;
	int				port;
	std::string		body;

	// PRIVATE HELPERS
	std::string 		receiveContent(int const& request_fd);
	std::size_t 		parseMethod(void);
	void 				decodeUri(void);
	std::size_t 		parseUri(std::size_t pos);
	std::size_t 		parseQueryString(std::size_t pos);
	std::size_t 		parseHttpVersion(std::size_t pos);
	std::size_t 		parseRequestLine(void);
	void 				parseHostFieldValue(std::string const& field_value);
	void 				parseHeaderField(std::string const& header_field);
	std::size_t 		parseHeaders(std::size_t pos);
	void 				parseContent(void);

	// NOT USED
	Request(void);

public:
	// CONSTRUCTOR & DESTRUCTOR
	Request(std::string const &content);
	Request(int const &request_fd);
	Request(Request const &r);
	~Request(void);

	// OPERATOR
	Request				&operator=(Request const &rhs);

	// GETTERS
	std::string const	&getContent(void) const;
	bool const			&getIsBad(void) const;
	HttpErrorType const	&getErrorType(void) const;
	HttpMethod const 	&getMethod(void) const;
	std::string const 	&getUri(void) const;
	std::string const 	&getQueryString(void) const;
	double const		&getHttpVersion(void) const;
	std::string const 	&getHost(void) const;
	int const			&getPort(void) const;
	std::string const	&getBody(void) const;

	// SETTERS

	void				setUri(std::string uri);

	// EXCEPTIONS
	class BadRequestException : public std::exception
	{
		virtual char const* what() const throw();
	};

	class VersionNotImplementedException : public std::exception
	{ 
		virtual char const* what() const throw();
	};
};

std::ostream &operator<<(std::ostream &os, Request const &req);

#endif
