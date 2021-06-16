#include "Response.hpp"

std::string const Response::server_ = "webserver/1.0";

Response::Response()
{
	return;
}

Response::Response(Response const& src)
{
	*this = src;
	return;
}

Response& Response::operator=(Response const& rhs)
{
	http_version_ = rhs.http_version_;
	status_code_ = rhs.status_code_;
	reason_phrase_ = rhs.reason_phrase_;
	content_type_ = rhs.content_type_;
	content_length_ = rhs.content_length_;
	body_ = rhs.body_;
	return *this;
}

std::string Response::readUri(std::string const& uri)
{
	std::ifstream ifs(("." + uri).c_str());

	if (ifs.is_open() == false) {
		return "open error";
	}

	std::string file_content;
	std::string buf;
	while (ifs.bad() == false && ifs.eof() == false) {
		std::getline(ifs, buf);
		file_content += buf;
		if (ifs.eof() == false)
			file_content += "\n";
	}
	return file_content;
}

Response::Response(Request const& request)
{
	http_version_ = request.http_version();
	body_ = readUri(request.uri());
	content_length_ = body_.length();
	status_code_ = 200;
	reason_phrase_ = "OK";
	content_type_ = "text/html";
	return;
}

Response::~Response()
{
	return;
}

std::string Response::toString() const
{
	std::stringstream ss;
	ss << "HTTP/" << http_version_ << " " << status_code_ << " "
		<< reason_phrase_ << "\r\nServer: " << server_
		<< "\r\nContent-Type: " << content_type_
		<< "\r\nContent-Length: " << content_length_ << "\r\n\r\n" << body_;
	return ss.str();
}
