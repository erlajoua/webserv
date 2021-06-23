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

Response::Response(Request & request, Server &server)
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


void		Response::createResponse(Request &request, Server &server)
{
	this->http_version = request.getHttpVersion();
	this->setStatusCode(request, server);
	this->setBody(server);
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
	else if (this->status_code == 301)
		this->reason_phrase = "Move Permanently";
	else if (this->status_code == 505)
		this->reason_phrase = "Version Not Supported";
	else if (this->status_code == 400)
		this->reason_phrase = "Bad Request";
	else if (this->status_code == 405)
		this->reason_phrase = "Method Not Allowed";
	else if (this->status_code == 413)
		this->reason_phrase = "Payload Too Large";
	else
		this->reason_phrase = "";
}

void		Response::setBody(Server &server)
{
	if (this->status_code != 200)
	{
		this->body = this->getErrorPage(server);
		return ;
	}
	
	this->body = getAllFile(this->full_path);

	if (this->body.length() > server.getClientBodySize())
	{
		this->status_code = 413;
		this->setBody(server);
	}
}

/* status_code */

void		Response::handleFolderPath(Request &request, Server &server)
{
	struct stat stats_path;
	std::string uri = server.getRoot() + request.getUri();

	try
	{
		Location location = this->getLocation(server, request.getUri());
		if (location.getIndex() != "none") //il y a un index
		{
			request.setUri(server.getRoot() + request.getUri() + location.getIndex());
			if (stat(request.getUri().c_str(), &stats_path) == 0)
			{
				if (stats_path.st_mode & S_IFREG) //file
					this->handleFilePath(request);
				else if (stats_path.st_mode & S_IFDIR) //folder
				{
					this->redirection_path = location.getIndex();
					this->status_code = 301;
				}
			}
			else
			{
				if (location.getAutoindex() == true)
					this->status_code = 666;
				else
					this->status_code = 403;
			}
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
}

void		Response::handleFilePath(Request &request)
{
	if (access(request.getUri().c_str(), R_OK) == 0)
	{
		this->full_path = request.getUri();
		this->status_code = 200;
	}
	else
		this->status_code = 403;
}

void		Response::setStatusCode(Request &request, Server &server)
{
	struct stat stats_path;
	std::string uri = server.getRoot() + request.getUri();

	if (this->checkMethodsAllowed(server, request) == 0)
		this->status_code = 405;
	else if (request.getIsBad() == true)
	{
		if (request.getErrorType() == kBadRequest)
			this->status_code = 400;
		else if (request.getErrorType() == kVersionNotImplemented)
			this->status_code = 505;
	}
	else if (stat(uri.c_str(), &stats_path) == 0)
	{
 		if (stats_path.st_mode & S_IFDIR)
			this->handleFolderPath(request, server);
		else if (stats_path.st_mode & S_IFREG)
		{
			request.setUri(server.getRoot() + request.getUri());
			this->handleFilePath(request);
		}
	}
	else
		this->status_code = 404;
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
		<< "\r\nContent-Type: " << this->content_type;
	if (this->status_code == 301)
		ss << "\r\nLocation: " << this->redirection_path;
	ss << "\r\nContent-Length: " << this->content_length << "\r\n\r\n" << this->body;
	return (ss.str());
}

std::string	Response::findCustomErrorPage(Server &server, int status_code)
{
	std::vector<std::string> erros_pages = server.getErrors();
	std::stringstream str_code;
	str_code << status_code;

	for (std::vector<std::string>::iterator it = erros_pages.begin(); it != erros_pages.end(); it++)
	{
		char *path = (char *)(*it).c_str();
		std::stringstream str_path;
		path = strrchr(path, '/');
		path++;
		str_path << path;

		if (str_path.str() == str_code.str() + ".html")
			return (server.getRoot() + "/" + *it);
	}
	return (DEFAULT_PATH_ERROR + str_code.str() + ".html");
}

std::string	Response::getErrorPage(Server &server)
{
	//refacto en 1 ligne, des que toutes les erreurs sont gérées // return (getAllFile(this->findCustomErrorPage(server, this->status_code)));
	if (this->status_code == 400)
		return (getAllFile(this->findCustomErrorPage(server, 400))); 
	if (this->status_code == 403)
		return (getAllFile(this->findCustomErrorPage(server, 403)));
	if (this->status_code == 404)
		return (getAllFile(this->findCustomErrorPage(server, 404)));
	if (this->status_code == 505)
		return (getAllFile(this->findCustomErrorPage(server, 505)));
	if (this->status_code == 413)
		return (getAllFile(this->findCustomErrorPage(server, 413)));
	if (this->status_code == 405)
		return (getAllFile(this->findCustomErrorPage(server, 405)));
	return (getAllFile("./www/error_pages/not_handled.html"));
}

//utils

int		Response::checkMethodsAllowed(Server &server, Request &request)
{
	Location location = this->getLocation(server, request.getUri());
	std::vector<std::string> methods = location.getMethods();
	for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++)
	{
		if (*it == "get" && request.getMethod() == kGet)
			return (1);
		if (*it == "post" && request.getMethod() == kPost)
			return (1);
		if (*it == "delete" && request.getMethod() == kDelete)
			return (1);
	}
	return (0);
}

int		Response::getPositionLastChar(char *str, char c) const
{
	int i = strlen(str);

	while (str[i] != c && i >= 0)
		i--;
	return (i);
}

Location&	Response::getLocation(Server &server, std::string uri)
{
	std::vector<Location> *locations = server.getLocations();
	char *ref = (char *)uri.c_str();
	int pos = 0;

	while (pos != -1)
	{
		pos = this->getPositionLastChar(ref, '/');
		for (std::vector<Location>::iterator it = locations->begin(); it != locations->end(); it++)
		{
			if (it->getPath() == ref)
				return (*it);
		}
		ref[pos] = 0;
	}
	for (std::vector<Location>::iterator it = locations->begin(); it != locations->end(); it++)
	{
		if (it->getPath() == "/")
			return (*it);
	}
	throw defaultLocationNotFound();
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

//exceptions

const char*		Response::IndexLocationNotFile::what() const throw()
{
	return "The index location is not a file, parsing??.\n";
}

const char*		Response::defaultLocationNotFound::what() const throw()
{
	return "The default location / is not found.\n";
}