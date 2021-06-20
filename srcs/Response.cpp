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
	this->http_version = request.getHttp_version();
	this->setStatusCode(request, server);
	this->setBody(request);
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

void		Response::setBody(Request const &request)
{
	
	if (this->status_code != 200)
	{
		this->body = this->getErrorPage();
		return ;
	}
	this->body = getAllFile("." + request.getUri());
}

#define REDIRECTION_PAGE "./index.html"

void		Response::setStatusCode(Request const &request, Server &server)
{
	std::ifstream input;
	std::string uri;
	int index_route;

	uri = server.getRoot() + request.getUri(); //       www    

	std::cout << "uri = " << uri << "\n";

	struct stat stats_path;

	if (stat(uri.c_str(), &stats_path) == 0)
	{
		if (stats_path.st_mode & S_IFDIR)
		{
			if ((index_route = server.hasRoute(uri)) >= 0) //a changer pour paser selon la route 
			{
				std::vector<Route> *routes = server.getRoutes();

				for (std::vector<Route>::iterator it = routes->begin(); it != routes->end() ; it++)
				{
					std::cout << it->getPath() << "\n";
				}
				this->status_code = 666; //auto-index
			}
			else
				this->status_code = 404; //pas bon
		}
		else if (stats_path.st_mode & S_IFREG)
		{
			std::cout << "TEST = " << uri.c_str() << "\n";
			if (access(uri.c_str(), R_OK) == 0)
				this->status_code = 200;
			else
				this->status_code = 403;
		}
	}
	else
		this->status_code = 404;
}

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