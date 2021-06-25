/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 11:31:55 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/22 20:37:16 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndex.hpp"

// CONSTRUCTOR & DESTRUCTOR

AutoIndex::AutoIndex(std::string const &root, std::string const &location_path): location_path(location_path), root(root){
	page_content = "<!DOCTYPE html><html><head><title>42 Webserv</title><meta charset=\"utf-8\" name=\"Webserv\" content=\"Webserv\"></head><body><h1>Index of " + location_path + "</h1><br/><hr><br/><table><td><h3>Path</h3></td><td style=\"padding-left: 250px;\"><h3>Last Modified</h3>";
}

AutoIndex::~AutoIndex(void){
}

// MEMBER FUNCTIONS

void				AutoIndex::buildAutoIndex(void)
{
	int		i;
	int		n;

	this->parseFiles();
	this->adjustFiles();
	n = this->files.size();

	i = 0;
	while (i < n)
	{
		this->addContentLine(i);
		i++;
	}
	this->page_content += "</table><br/><hr></body></html>";
}

// PRIVATE HELPERS

void				AutoIndex::parseFiles(void) {
	DIR				*dir;
	struct dirent	*file;
	std::string		concat;

	if (location_path == "/")
		concat = this->root + this->location_path;
	else
		concat = this->root + "/" + this->location_path;
	dir = opendir(concat.c_str());
	if (!dir)
		return ;
	file = readdir(dir);
	while (file)
	{
		this->files.push_back(file->d_name);
		file = readdir(dir);
	}
	closedir(dir);
	this->processFiles();
}

bool				AutoIndex::sortHelper(std::string const &file1, std::string const &file2) {
	if (file1[file1.size() - 1] == '/' && file2[file2.size() - 1] != '/')
		return (true);
	else if (file1[file1.size() - 1] != '/' && file2[file2.size() - 1] == '/')
		return (false);
	else
		return (file1 < file2);
}

bool				AutoIndex::isDir(std::string const &name) {
	struct stat tmp_stat;
	if (stat(name.c_str(), &tmp_stat) == 0)
	{
		if (tmp_stat.st_mode & S_IFDIR)
			return (true);
	}
	return (false);
}

void				AutoIndex::processFiles(void) {
	std::vector<std::string>::iterator first = this->files.begin();
	std::vector<std::string>::iterator last = this->files.end();
	std::string	concat;
	concat = this->root + this->location_path + "/";
	while (first != last)
	{
		if ((*first)[0] == '.')
		{
			first = this->files.erase(first);
			last = this->files.end();
		}
		else
		{
			if (this->isDir(concat + *first))
				*first = ((*first)[(*first).size() - 1] == '/') ? *first : *first = *first + "/";
			first++;
		}
	}
	std::sort(this->files.begin(), this->files.end(), sortHelper);
}

void				AutoIndex::adjustFiles(void) {
	this->adj_files = this->files;
	for (std::vector<std::string>::iterator it = this->adj_files.begin(); it != this->adj_files.end(); it++)
		*it = (this->location_path[this->location_path.size() - 1] == '/') ? this->location_path + *it : this->location_path + "/" + *it;
}

std::string			AutoIndex::getLastModified(std::string const &path) {
	struct stat		tmp_stat;
	char			buffer[100];

	if (stat(path.c_str(), &tmp_stat) == 0)
	{
		strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&tmp_stat.st_mtime));
		return (buffer);
	}
	return ("");
}

void				AutoIndex::addContentLine(int i) {
	std::string concat;
	concat = "<tr><td><a href=\"" + this->adj_files[i] + "\">" + this->files[i] + "</a></td><td style=\"padding-left: 250px;\">" + this->getLastModified(this->root + this->adj_files[i]) + "</td></tr>";
	this->page_content += concat; 
}

// GETTERS

std::string const 	&AutoIndex::getPageContent(void) const { 
	return (this->page_content);
}
