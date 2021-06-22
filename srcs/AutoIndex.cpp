/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 11:31:55 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/22 11:31:56 by nessayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/AutoIndex.hpp"

// CONSTRUCTOR & DESTRUCTOR

AutoIndex::AutoIndex(std::string const &root, std::string const &location_path): location_path(location_path), root(root){
	index = "<!DOCTYPE html><html><body><h1>Index of " + location_path + "</h1><br/><hr><br/><table><td><h3>Path</h3></td><td style=\"padding-left: 250px;\"><h3>Last Modified</h3>";
}

AutoIndex::~AutoIndex(void){
}

// MEMBER FUNCTIONS

void				AutoIndex::createIndex(void)
{
	int		i;
	int		n;

	this->getFilenames();
	//for (std::vector<std::string>::iterator it = this->filenames.begin(); it != this->filenames.end(); it++)
	//	std::cout << YELLOW << *it << RESET << std::endl; 
	this->createEntries();
	//for (std::vector<std::string>::iterator it = this->entries.begin(); it != this->entries.end(); it++)
	//	std::cout << GREEN << *it << RESET << std::endl; 
	n = this->filenames.size();

	i = 0;
	while (i < n)
	{
		this->addIndexLine(i);
		i++;
	}
	this->index += "</table><br/><hr></body></html>";
}

// PRIVATE HELPERS

void				AutoIndex::getFilenames(void) {
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
		//std::cout << RED << file->d_name << RESET << std::endl; 
		this->filenames.push_back(file->d_name);
		file = readdir(dir);
	}
	closedir(dir);
	this->processFilenames();
}

bool				AutoIndex::compareFilenames(std::string const &entry_a, std::string const &entry_b) {
	if (entry_a[entry_a.size() - 1] == '/' && entry_b[entry_b.size() - 1] != '/')
		return (true);
	else if (entry_a[entry_a.size() - 1] != '/' && entry_b[entry_b.size() - 1] == '/')
		return (false);
	else
		return (entry_a < entry_b);
}

bool				AutoIndex::isDirectory(std::string const &name) {
	struct stat tmp_stat;
	if (stat(name.c_str(), &tmp_stat) == 0)
	{
		if (tmp_stat.st_mode & S_IFDIR)
			return (true);
	}
	return (false);
}

void				AutoIndex::processFilenames(void) {
	std::vector<std::string>::iterator first = this->filenames.begin();
	std::vector<std::string>::iterator last = this->filenames.end();
	std::string	concat;
	concat = this->root + this->location_path + "/";
	while (first != last)
	{
		if ((*first)[0] == '.')
		{
			first = this->filenames.erase(first);
			last = this->filenames.end();
		}
		else
		{
			//std::cout << BLUE << concat + *first << RESET << std::endl;
			if (this->isDirectory(concat + *first))
				*first = ((*first)[(*first).size() - 1] == '/') ? *first : *first = *first + "/";
			first++;
		}
	}
	sort(this->filenames.begin(), this->filenames.end(), compareFilenames);
}

void				AutoIndex::createEntries(void) {
	this->entries = this->filenames;
	for (std::vector<std::string>::iterator it = this->entries.begin(); it != this->entries.end(); it++)
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

void				AutoIndex::addIndexLine(int i) {
	std::string concat;
	//std::cout << RED << this->root + this->entries[i] << RESET << std::endl;
	concat = "<tr><td><a href=\"" + this->entries[i] + "\">" + this->filenames[i] + "</a></td><td style=\"padding-left: 250px;\">" + this->getLastModified(this->root + this->entries[i]) + "</td></tr>";
	//std::cout << YELLOW << concat << RESET << std::endl; 
	this->index += concat; 
}

// GETTERS

std::string const 	&AutoIndex::getIndex(void) const { 
	return (this->index);
}
