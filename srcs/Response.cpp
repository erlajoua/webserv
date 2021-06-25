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
	if (this->status_code != 200)
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
	envp[kContentType] =
		new char[std::strlen("CONTENT_TYPE=application/x-www-form-urlencoded") + 1];
	std::strcpy(envp[kContentType],
			"CONTENT_TYPE=application/x-www-form-urlencoded");
	std::stringstream content_length;
	content_length << "CONTENT_LENGTH=" << request.getBody().length();
	envp[kContentLength] =
		new char[content_length.str().length() + 1];
	std::strcpy(envp[kContentLength], content_length.str().c_str());
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
	}
}

std::string Response::readCgiOutput(int pipes_fds[2][2])
{
	std::string output = "";
	char buf[1024];
	size_t bytes_read = read(pipes_fds[1][0], buf, 1024);
	while (bytes_read > 0)
	{
		output += std::string(buf, bytes_read);
		bytes_read = read(pipes_fds[1][0], buf, 1024);
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
		if (wait(NULL) == -1)
		{
			this->closeCgiPipes(pipes_fds);
			throw std::exception();
		}
	}
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
	std::size_t content_type_value_pos = output.find(' ') + 1;
	std::size_t semicolon_pos = output.find(';', content_type_value_pos);
	if (content_type_value_pos - 1 == output.npos
			|| semicolon_pos == output.npos)
	{
		throw std::exception();
	}
	this->cgi_content_type = output.substr(content_type_value_pos,
			semicolon_pos - content_type_value_pos);
	std::size_t body_pos = output.find("\n\r\n", semicolon_pos) + 3;
	if (body_pos - 3 == output.npos)
	{
		throw std::exception();
	}
	this->deleteCgiEnvVar(envp);
	return output.substr(body_pos);
}

void		Response::setBody(char **envp, std::string const &uri,
		Request const &request, Server &server)
{
	/*if (this->status_code == 200 && this->is_autoindex == true)
	{
		AutoIndex a("./www", "/even_pages");
		a.buildAutoIndex();
		this->body = a.getPageContent();
		return ;
	}*/
	if (this->status_code != 200)
	{
		this->body = this->getErrorPage(server);
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
			// TODO: 500 response
		}
	}
	else
	{
		this->body = getAllFile(this->full_path);
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
				{
					this->status_code = 666;
					//this->status_code = 200;
					//this->is_autoindex = true;
				}
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
	int pos;

	for (std::vector<std::string>::iterator it = erros_pages.begin(); it != erros_pages.end(); it++)
	{
		std::string path(*it);
		pos = path.rfind('/');
		if (pos == -1)
			break ;
		path.assign(path.begin() + pos + 1, path.end());
		if (path == str_code.str() + ".html")
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
