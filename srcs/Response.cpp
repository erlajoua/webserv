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

/*void		Response::handleUnknownPath(Request const &request, Server &server)
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
}*/

void		Response::handleFolderPath(Request &request, Server &server)
{
	struct stat stats_path;
	std::string uri = server.getRoot() + request.getUri();

	std::cout << RED << "FOLDER" << RESET << std::endl;
	try
	{
		Location location = this->getLocation(server, request.getUri());
		if (location.getIndex() != "none") //il y a un index
		{
			std::cout << RED << "Old uri = " << request.getUri() << RESET << "\n";
			request.setUri(server.getRoot() + request.getUri() + location.getIndex());
			std::cout << RED << "Index exist, new uri = " << request.getUri() << RESET << std::endl;

			if (stat(request.getUri().c_str(), &stats_path) == 0)
			{
				if (stats_path.st_mode & S_IFREG) //file
				{
					std::cout << RED << "New uri is a file = " << RESET << std::endl;
					std::cout << RED << "Handle File path with uri = " << request.getUri() << RESET << "\n";
					this->handleFilePath(request, server);
				}
				else if (stats_path.st_mode & S_IFDIR) //folder
				{
					std::cout << RED << "New uri is a folder = " << RESET << std::endl;
					std::cout << YELLOW << "Implementation 301" << RESET << "\n";
					this->status_code = 301;
				}
			}
			else
			{
				if (location.getAutoindex() == true)
				{
					std::cout << YELLOW << "Implementation auto index" << RESET << "\n";
				}
				else
					status_code = 403;
			}
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
}

void		Response::handleFilePath(Request &request, Server &server)
{
	(void)server;
	std::cout << RED << "FILE" << RESET << std::endl;
	if (access(request.getUri().c_str(), R_OK) == 0)
	{
		std::cout << RED << "FILE -> OK" << RESET << std::endl;
		this->full_path = request.getUri();
		this->status_code = 200;
	}
	else
	{
		std::cout << RED << "FILE -> Forbidden" << RESET << std::endl;
		this->status_code = 403;
	}
}

void		Response::setStatusCode(Request &request, Server &server)
{
	struct stat stats_path;
	std::string uri = server.getRoot() + request.getUri();
	std::cout << RED << uri << RESET << std::endl;

	if (stat(uri.c_str(), &stats_path) == 0)
	{
 		if (stats_path.st_mode & S_IFDIR)
			this->handleFolderPath(request, server);
		else if (stats_path.st_mode & S_IFREG)
		{
			request.setUri(server.getRoot() + request.getUri());
			this->handleFilePath(request, server);
		}
	}
	else
	{
		std::cout << RED << "Unknow path" << RESET << std::endl;
		this->status_code = 404;
	}
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

//utils

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