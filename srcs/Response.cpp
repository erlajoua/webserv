#include "../includes/Response.hpp"

std::string const Response::server = "webserver/1.0";

//constructors

Response::Response()
{
}

Response::Response(Response const& src)
{
	*this = src;
}

Response::Response(Request const& request, Server &server)
{
	if (request.getMethod() == kGet)
	{
		createResponse(request, server);
	}
	else
	{
		std::cout << "Method POST and DELETE not implemented yet\n";
	}
}

//destructors

Response::~Response()
{
}

//methods


void		Response::createResponse(Request const &request, Server &server)
{
	this->http_version = request.getHttpVersion();
	this->setStatusCode(request, server);
	this->setBody(request, server);
	this->setReasonPhrase();
	this->setContentLength();
	this->setContentType();
}

void		Response::setContentType(void)
{
	this->content_type = "text/html";
}

void		Response::setReasonPhrase(void)
{
	if (this->status_code == 200)
		this->reason_phrase = "OK";
	else if (this->status_code == 403)
		this->reason_phrase = "Forbidden";
	else if (this->status_code == 404)
		this->reason_phrase = "Not Found";
	else
		this->reason_phrase = "";
}

void		Response::setBody(Request const &request, Server &server)
{
	(void)server;
	(void)request;
	if (this->status_code != 200)
	{
		this->body = this->getErrorPage();
		return ;
	}
	this->body = getAllFile(this->full_path);
}

/* status_code */

void		Response::handleUnknownPath(Request const &request, Server &server)
{
	std::vector<Location> *Locations = server.getLocations();
	std::vector<Location>::iterator it;

	for (it = Locations->begin(); it != Locations->end() ; it++)
	{
		if (it->getPath() == request.getUri())
			break ;
	}
	if (it == Locations->end())
		this->status_code = 404;
	else
	{
		if (access(it->getIndex().c_str(), R_OK == 0))
		{
			this->full_path = server.getRoot() + "/" + it->getIndex();
			this->status_code = 200;
		}
		else
			this->status_code = 666; //auto-index ?
	}
}

void		Response::handleFolderPath(Request const &request, Server &server)
{
	int index_Location;
	std::string uri = server.getRoot() + request.getUri();

	std::cout << RED << uri << RESET << std::endl;
	if ((index_Location = server.hasLocation(uri)) >= 0)
	{
		std::vector<Location> *Locations = server.getLocations();
		std::vector<Location>::iterator it;
		for (it = Locations->begin(); it != Locations->end() ; it++)
		{
			if (it->getPath() == request.getUri())
				break ;
		}
		this->full_path = server.getRoot() + request.getUri() + it->getIndex();
		if (access(this->full_path.c_str(), R_OK) == 0)
			this->status_code = 200;
		else
			this->status_code = 666;
	}
	else
		this->status_code = 404;
}

void		Response::handleFilePath(Request const &request, Server &server)
{
	std::string uri = server.getRoot() + request.getUri();

	if (access(uri.c_str(), R_OK) == 0)
	{
		this->full_path = uri;
		this->status_code = 200;
	}
	else
		this->status_code = 403;
}

void		Response::setStatusCode(Request const &request, Server &server)
{
	struct stat stats_path;
	std::string uri = server.getRoot() + request.getUri();
	std::cout << RED << uri << RESET << std::endl;

	if (stat(uri.c_str(), &stats_path) == 0)
	{
 		if (stats_path.st_mode & S_IFDIR)
			this->handleFolderPath(request, server);
		else if (stats_path.st_mode & S_IFREG)
			this->handleFilePath(request, server);
	}
	else
		this->handleUnknownPath(request, server);
}

/* */

void		Response::setContentLength(void)
{
	this->content_length = this->body.length();
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

std::string	Response::getErrorPage(void)
{
	if (this->status_code == 403)
		return (getAllFile("./www/error_pages/403.html"));
	if (this->status_code == 404)
		return (getAllFile("./www/error_pages/404.html"));
	return (getAllFile("./www/error_pages/not_handled.html"));
}

//overloads

Response	&Response::operator=(Response const& rhs)
{
	this->http_version = rhs.http_version;
	this->status_code = rhs.status_code;
	this->reason_phrase = rhs.reason_phrase;
	this->content_type = rhs.content_type;
	this->content_length = rhs.content_length;
	this->body = rhs.body;
	return (*this);
}
