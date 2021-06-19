#include "../includes/Response.hpp"

# define ROOT_PATH "./"

std::string const Response::server = "webserver/1.0";

//constructors

Response::Response()
{
}

Response::Response(Response const& src)
{
	*this = src;
}

Response::Response(Request const& request)
{
	if (request.getMethod() == kGet)
	{
		createResponse(request);
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


void		Response::createResponse(Request const &request)
{
	this->http_version = request.getHttp_version();
	this->setStatusCode(request);
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
	this->body = getAllFile(ROOT_PATH + request.getUri());
}

#define REDIRECTION_PAGE "./index.html"

void		Response::setStatusCode(Request const &request)
{
	std::ifstream input;
	std::string uri;

	uri = ROOT_PATH + request.getUri();

	if (request.getUri() != "./")
		input.open(uri.c_str());
	else
		input.open(REDIRECTION_PAGE);
			
	if (access(uri.c_str(), F_OK) == 0)
	{
		if (access(uri.c_str(), R_OK) == 0)
			this->status_code = 200;
		else
			this->status_code = 403;
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