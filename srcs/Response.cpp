#include "Response.hpp"

std::string const Response::server = "webserver/1.0";

//constructors

Response::Response()
{
}

Response::Response(Response const& src)
{
	*this = src;
}

Response::Response(char **envp, Request & request, Server &server) :
	is_autoindex(false), cgi_content_type("")
{
	createResponse(envp, request, server);
}

//destructors

Response::~Response()
{
}

//methods


void		Response::createResponse(char **envp, Request &request, Server &server)
{
	std::string uri = request.getUri();
	this->http_version = request.getHttpVersion();
	this->setStatusCode(request, server);
	this->setBody(envp, uri, request, server);
	this->setReasonPhrase();
	this->setContentLength();
	this->setContentType();
}

void		Response::setContentType(void)
{
	std::size_t dot_pos = this->full_path.rfind('.');
	if (this->status_code == 200 && this->is_autoindex == true)
	{
		this->content_type = "text/html";
	}
	else if (this->status_code != 200)
	{
		this->content_type = "text/html";
	}
	else if (this->cgi_content_type != "")
	{
		this->content_type = this->cgi_content_type;
	}
	else if (this->full_path.find(".html", dot_pos) != std::string::npos)
	{
		this->content_type = "text/html";
	}
	else if (this->full_path.find(".css", dot_pos) != std::string::npos)
	{
		this->content_type = "text/css";
	}
	else
	{
		this->content_type = "text/plain";
	}
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
	else if (this->status_code == 302)
		this->reason_phrase = "Found";
	else if (this->status_code == 303)
		this->reason_phrase = "See Other";
	else if (this->status_code == 304)
		this->reason_phrase = "Not Modified";
	else if (this->status_code == 307)
		this->reason_phrase = "Temporary Redirect";
	else if (this->status_code == 308)
		this->reason_phrase = "Permanent Redirect";
	else if (this->status_code == 505)
		this->reason_phrase = "Version Not Supported";
	else if (this->status_code == 400)
		this->reason_phrase = "Bad Request";
	else if (this->status_code == 405)
		this->reason_phrase = "Method Not Allowed";
	else if (this->status_code == 413)
		this->reason_phrase = "Payload Too Large";
	else if (this->status_code == 204)
		this->reason_phrase = "No Content";
	else
		this->reason_phrase = "";
}

void		Response::setCgiEnvVar(char **envp, Request const &request,
		std::string const& script_filename)
{
	envp[kScriptFilename] =
		new char[("SCRIPT_FILENAME=" + script_filename).length() + 1];
	std::strcpy(envp[kScriptFilename],
			("SCRIPT_FILENAME=" + script_filename).c_str());
	envp[kRedirectStatus] = new char[std::strlen("REDIRECT_STATUS=200") + 1];
	std::strcpy(envp[kRedirectStatus], "REDIRECT_STATUS=200");
	if (request.getMethod() == kGet)
	{
		envp[kRequestMethod] = new char[std::strlen("REQUEST_METHOD=GET") + 1];
		std::strcpy(envp[kRequestMethod], "REQUEST_METHOD=GET");
	}
	else if (request.getMethod() == kPost)
	{
		envp[kRequestMethod] = new char[std::strlen("REQUEST_METHOD=POST") + 1];
		std::strcpy(envp[kRequestMethod], "REQUEST_METHOD=POST");
	}
	envp[kQueryString] =
			new char[("QUERY_STRING=" + request.getQueryString()).length() + 1];
	std::strcpy(envp[kQueryString],
			("QUERY_STRING=" + request.getQueryString()).c_str());
	envp[kContentType] = new char[("CONTENT_TYPE=" + request.getContentType()).length() + 1];
	std::strcpy(envp[kContentType],
			("CONTENT_TYPE=" + request.getContentType()).c_str());
	std::stringstream content_length;
	content_length << "CONTENT_LENGTH=" << request.getBody().length();
	envp[kContentLength] =
		new char[content_length.str().length() + 1];
	std::strcpy(envp[kContentLength], content_length.str().c_str());
	envp[kHttpCookie] =
			new char[("HTTP_COOKIE=" + request.getCookie()).length() + 1];
	std::strcpy(envp[kHttpCookie],
			("HTTP_COOKIE=" + request.getCookie()).c_str());
}

void		Response::deleteCgiEnvVar(char **envp)
{
	delete[] envp[kScriptFilename];
	envp[kScriptFilename] = NULL;
	delete[] envp[kRedirectStatus];
	envp[kRedirectStatus] = NULL;
	delete[] envp[kRequestMethod];
	envp[kRequestMethod] = NULL;
	delete[] envp[kQueryString];
	envp[kQueryString] = NULL;
	delete[] envp[kContentLength];
	envp[kContentLength] = NULL;
	delete[] envp[kContentType];
	envp[kContentType] = NULL;
	delete[] envp[kHttpCookie];
	envp[kHttpCookie] = NULL;
}

void		Response::closeCgiPipes(int pipes_fds[2][2])
{
	if (pipes_fds[0][0] != -1)
	{
		close(pipes_fds[0][0]);
		pipes_fds[0][0] = -1;
	}
	if (pipes_fds[0][1] != -1)
	{
		close(pipes_fds[0][1]);
		pipes_fds[0][1] = -1;
	}
	if (pipes_fds[1][0] != -1)
	{
		close(pipes_fds[1][0]);
		pipes_fds[1][0] = -1;
	}
	if (pipes_fds[1][1] != -1)
	{
		close(pipes_fds[1][1]);
		pipes_fds[1][1] = -1;
	}
}

void		Response::setCgiPipes(int pipes_fds[2][2])
{
	if (pipe(pipes_fds[0]) == -1)
	{
		this->closeCgiPipes(pipes_fds);
		throw std::exception();
	}
	else if (pipe(pipes_fds[1]) == -1)
	{
		this->closeCgiPipes(pipes_fds);
		throw std::exception();
	}
}

void		Response::dupCgiProcessStreams(std::string const &request_body,
		int pipes_fds[2][2])
{
	write(pipes_fds[0][1], request_body.c_str(), request_body.length());
	if (dup2(pipes_fds[0][0], STDIN_FILENO) == -1)
	{
		throw std::exception();
	}
	if (dup2(pipes_fds[1][1], STDOUT_FILENO) == -1)
	{
		throw std::exception();
	}
	this->closeCgiPipes(pipes_fds);
}

void		Response::cgiProcess(char **envp, std::string const &request_body,
		std::string const &cgi_bin, int pipes_fds[2][2])
{
	char *pathname = NULL;
	char *argv[2] = {NULL, NULL};
	try
	{
		this->dupCgiProcessStreams(request_body, pipes_fds);
		pathname = new char[cgi_bin.length() + 1];
		std::strcpy(pathname, cgi_bin.c_str());
		argv[0] = new char[cgi_bin.length() + 1];
		std::strcpy(argv[0], cgi_bin.c_str());
		execve(pathname, argv, envp);
		throw std::exception();
	}
	catch (...)
	{
		this->deleteCgiEnvVar(envp);
		this->closeCgiPipes(pipes_fds);
		delete[] pathname;
		delete[] argv[0];
		exit(1);
	}
}

std::string Response::readCgiOutput(int pipes_fds[2][2])
{
	std::string output = "";
	char buf[1024];
	ssize_t bytes_read = read(pipes_fds[1][0], buf, 1024);
	while (bytes_read > 0)
	{
		output += std::string(buf, bytes_read);
		bytes_read = read(pipes_fds[1][0], buf, 1024);
	}
	if (bytes_read == -1)
	{
		throw std::exception();
	}
	return (output);
}

void		Response::callCgi(char **envp, Request const &request,
		std::string const& cgi_bin, int pipes_fds[2][2])
{
	int pid = fork();
	if (pid == -1)
	{
		this->closeCgiPipes(pipes_fds);
		throw std::exception();
	}
	else if (pid == 0)
	{
		this->cgiProcess(envp, request.getBody(), cgi_bin, pipes_fds);
		exit(1);
	}
	else
	{
		close(pipes_fds[0][0]);
		pipes_fds[0][0] = -1;
		close(pipes_fds[0][1]);
		pipes_fds[0][1] = -1;
		close(pipes_fds[1][1]);
		pipes_fds[1][1] = -1;
		int wstatus;
		if (wait(&wstatus) == -1)
		{
			this->closeCgiPipes(pipes_fds);
			throw std::exception();
		}
		if (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0)
			throw std::exception();
	}
}

std::size_t	Response::parseCgiOutputHeaders(std::string const &output)
{
	std::size_t crlf_pos = 0;
	std::size_t set_cookie_value_pos = output.find("Set-Cookie: ") + 12;
	while (set_cookie_value_pos - 12 != output.npos)
	{
		crlf_pos = output.find("\r\n", set_cookie_value_pos);
		if (crlf_pos == output.npos)
		{
			throw std::exception();
		}
		set_cookie_values.push_back(output.substr(set_cookie_value_pos,
					crlf_pos - set_cookie_value_pos));
		set_cookie_value_pos = output.find("Set-Cookie: ", set_cookie_value_pos) + 12;
	}
	std::size_t content_type_value_pos = output.find("Content-type: ") + 14;
	crlf_pos = output.find("\r\n", content_type_value_pos);
	if (content_type_value_pos - 14 == output.npos
			|| crlf_pos == output.npos)
	{
		throw std::exception();
	}
	this->cgi_content_type = output.substr(content_type_value_pos,
			crlf_pos - content_type_value_pos);
	std::size_t body_pos = output.find("\n\r\n", crlf_pos) + 3;
	if (body_pos - 3 == output.npos)
	{
		throw std::exception();
	}
	return (body_pos);
}

std::string Response::getCgiOutputBody(char **envp, Request const &request,
		std::string const& script_filename, std::string const& cgi_bin)
{
	this->setCgiEnvVar(envp, request, script_filename);
	int pipes_fds[2][2] = { {-1, -1}, {-1, -1} };
	this->setCgiPipes(pipes_fds);
	this->callCgi(envp, request, cgi_bin, pipes_fds);
	std::string output = this->readCgiOutput(pipes_fds);
	this->closeCgiPipes(pipes_fds);
	std::size_t body_pos = this->parseCgiOutputHeaders(output);
	this->deleteCgiEnvVar(envp);
	return output.substr(body_pos);
}

void		Response::setBody(char **envp, std::string const &uri,
		Request const &request, Server &server)
{
	if (this->status_code == 200 && this->is_autoindex == true)
	{
		AutoIndex a(server.getRoot(), request.getUri());
		a.buildAutoIndex();
		this->body = a.getPageContent();
		return ;
	}
	if (this->status_code != 200 || request.getMethod() == kDelete)
	{
		this->body = this->findCustomErrorPage(server, this->status_code);
		return ;
	}
	Location const &location = getLocation(server, uri);
	std::string cgi_extension = location.getCgiExtension();
	if (this->full_path.length() >= cgi_extension.length()
			&& this->full_path.substr(this->full_path.length()
				- cgi_extension.length()) == cgi_extension)
	{
		std::string cgi_bin = server.getRoot() + location.getPath();
		if (location.getPath() != "/")
		{
			cgi_bin += "/";
		}
		cgi_bin += location.getCgiBin();
		try
		{
			this->body = this->getCgiOutputBody(envp, request, this->full_path,
					cgi_bin);
		}
		catch (...)
		{
			this->deleteCgiEnvVar(envp);
			this->status_code = 500;
			this->body = this->findCustomErrorPage(server, this->status_code);
		}
	}
	else
	{
		this->body = getAllFile(this->full_path);
	}
}

/* status_code */

void		Response::handleAutoIndex(Location &location)
{
	if (location.getAutoindex() == true)
	{
		this->status_code = 200;
		this->is_autoindex = true;
	}
	else
		this->status_code = 403;
}

void		Response::handleFolderPath(Request &request, Server &server)
{
	struct stat stats_path;
	std::string uri = server.getRoot() + request.getUri();
	std::string saveUri(request.getUri());

	try
	{
		Location location = this->getLocation(server, request.getUri());
		if (location.getIndex() != "none") //il y a un index
		{
			request.setUri(server.getRoot() + request.getUri() + "/" + location.getIndex());
			if (stat(request.getUri().c_str(), &stats_path) == 0)
			{
				if (stats_path.st_mode & S_IFREG) //file
					this->handleFilePath(request);
				else if (stats_path.st_mode & S_IFDIR) //folder
				{
					this->status_code = location.getRedirection();

					size_t pos = saveUri.rfind('/');
					if (saveUri !=  "/" && pos != saveUri.length() - 1)
						this->redirection_path = saveUri + "/";
					else
						this->redirection_path = location.getIndex();
				}
			}
			else
				this->handleAutoIndex(location);
		}
		else
			this->handleAutoIndex(location);

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

	if (request.getIsBad() == true)
	{
		if (request.getErrorType() == kBadRequest)
			this->status_code = 400;
		else if (request.getErrorType() == kVersionNotImplemented)
			this->status_code = 505;
	}
	else if (this->checkMethodsAllowed(server, request) == 0)
	{
		this->status_code = 405;
	}
	else if (request.getMethod() == kDelete)
	{
		if (remove(uri.c_str()) == 0)
			this->status_code = 200;
		else
			this->status_code = 204;
	}
	else if (request.getBody().length() > server.getClientBodySize())
	{
		this->status_code = 413;
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
		<< this->reason_phrase << "\r\nServer: " << this->server << "\r\n";
	for (std::vector<std::string>::const_iterator it = this->set_cookie_values.begin(),
			ite = this->set_cookie_values.end(); it != ite; ++it)
	{
		ss << "Set-Cookie: " << *it << "\r\n";
	}
	ss << "Content-Type: " << this->content_type;
	if (this->status_code == 301 || this->status_code == 302
	|| this->status_code == 303 || this->status_code == 304
	|| this->status_code == 307 || this->status_code == 308)
		ss << "\r\nLocation: " << this->redirection_path;
	ss << "\r\nContent-Length: " << this->content_length << "\r\n\r\n" << this->body;
	return (ss.str());
}

std::string Response::buildStandardErrorPage(int status_code)
{
	std::string content_page;
	(void) status_code;
	content_page = "<!DOCTYPE html><html><head><title>42 Webserv</title><meta charset=\"utf-8\" name=\"Webserv\" content=\"Webserv\"></head><body>";
	if (this->status_code == 403)
		content_page += "<h1>403 Forbidden</h1><p>Permission denied.</p>";
	else if (this->status_code == 404)
		content_page += "<h1>404 Not Found</h1><p>File not found.</p>";
	else if (this->status_code == 301)
		content_page += "<h1>301 Moved Permanently</h1>This and all future requests should be directed to the given URI.</p>";
	else if (this->status_code == 505)
		content_page += "<h1>505 HTTP Version Not Supported</h1><p>The server does not support the HTTP protocol version used in the request.</p>";
	else if (this->status_code == 400)
		content_page += "<h1>400 Bad Request</h1><p>Malformed or illegal request.</p>";
	else if (this->status_code == 405)
		content_page += "<h1>405 Method Not Allowed</h1><p>A request method is not supported for the requested resource.</p>";
	else if (this->status_code == 413)
		content_page += "<h1>413 Payload Too Large</h1><p>The request is larger than the server is willing or able to process.</p>";
	else if (this->status_code == 204)
		content_page += "<h1>204 No Content</h1><p>The server has nothing to delete.</p>";
	else if (this->status_code == 200)
		content_page += "<h1>200 Delete</h1><p>File deleted.</p>";
	else if (this->status_code == 500)
		content_page += "<h1>500 Server internal error</h1><p>The server experienced an error.</p>";
	else
		content_page += "<h1>666 Error not handled</h1><p>Work in progress...</p>";
	content_page += "</body></html>";
	return (content_page);
}

std::string	Response::findCustomErrorPage(Server &server, int status_code)
{
	std::vector<std::string> erros_pages = server.getErrors();
	std::stringstream str_code;
	str_code << status_code;
	int pos;

	if (this->status_code != 200)
	{
		for (std::vector<std::string>::iterator it = erros_pages.begin(); it != erros_pages.end(); it++)
		{
			std::string path(*it);
			pos = path.rfind('/');
			if (pos == -1)
				break ;
			path.assign(path.begin() + pos + 1, path.end());
			if (path == str_code.str() + ".html")
				return (getAllFile(server.getRoot() + "/" + *it));
		}
	}
	return (buildStandardErrorPage(status_code));
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

Location&	Response::getLocation(Server &server, std::string uri)
{
	std::vector<Location> *locations = server.getLocations();
	std::string ref(uri);
	int pos = 0;

	while (pos != -1)
	{
		pos = ref.rfind('/');
		for (std::vector<Location>::iterator it = locations->begin(); it != locations->end(); it++)
		{
			if (it->getPath() == ref)
				return (*it);
		}
		if (pos != -1)
			ref.assign(ref.begin(), ref.begin() + pos);
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
	this->is_autoindex = rhs.is_autoindex;
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
