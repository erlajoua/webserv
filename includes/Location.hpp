/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 17:46:49 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/22 20:37:05 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <cstdlib>
# include <string>
# include <vector>
# include <sstream>
# include <iostream>
# include <sys/stat.h>
# include <iterator>

# define RESET   		"\033[0m"
# define BLACK   		"\033[30m"      		/* Black */
# define RED     		"\033[31m"      		/* Red */
# define GREEN   		"\033[32m"      		/* Green */
# define YELLOW  		"\033[33m"      		/* Yellow */
# define BLUE    		"\033[34m"      		/* Blue */
# define MAGENTA 		"\033[35m"      		/* Magenta */
# define CYAN    		"\033[36m"      		/* Cyan */
# define WHITE   		"\033[37m"      		/* White */
# define BOLDBLACK		"\033[1m\033[30m"      	/* Bold Black */
# define BOLDRED		"\033[1m\033[31m"      	/* Bold Red */
# define BOLDGREEN		"\033[1m\033[32m"      	/* Bold Green */
# define BOLDYELLOW		"\033[1m\033[33m"      	/* Bold Yellow */
# define BOLDBLUE		"\033[1m\033[34m"      	/* Bold Blue */
# define BOLDMAGENTA	"\033[1m\033[35m"      	/* Bold Magenta */
# define BOLDCYAN		"\033[1m\033[36m"      	/* Bold Cyan */
# define BOLDWHITE		"\033[1m\033[37m"      	/* Bold White */

class Location
{
private:
	// ATTRIBUTES
	std::string						path;
	std::vector<std::string> 		methods;
	int 							redirection;
	std::string						index;
	bool							autoindex;
	std::string						cgi_extension;
	std::string						cgi_bin;
	std::string						upload_dir;

	// PRIVATE HELPERS
	bool							isMethodDeclared(std::vector<std::string> methods, std::string word);

public:
	// CONSTRUCTOR & DESTRUCTOR
	Location(void);
	Location(Location const &r);
	~Location(void);

	// OPERATOR
	Location						&operator=(Location const &r);

	// GETTERS
	std::string						getPath(void) const;
	std::vector<std::string> 		&getMethods(void);
	int								getRedirection(void) const;
	std::string						getIndex(void) const;
	bool							getAutoindex(void) const;
	std::string						getCgiExtension(void) const;
	std::string						getCgiBin(void) const;
	std::string						getUploadDir(void) const;

	// SETTERS
	void							setPath(std::string const &field);
	void							setMethods(std::string const &field);
	void							setRedirection(std::string const &field);
	void							setIndex(std::string const &root, std::string const &field);
	void							setAutoindex(std::string const &field);
	void							setCgiExtension(std::string const &field);
	void							setCgiBin(std::string const &root, std::string const &field);
	void							setUploadDir(std::string const &root, std::string const &field);

	// EXCEPTIONS
	class 		InvalidPathException: public std::exception
	{
		virtual const char* what() const throw();
	};
	class 		InvalidMethodsException: public std::exception
	{
		virtual const char* what() const throw();
	};
	class 		SameMethodException: public std::exception
	{
		virtual const char* what() const throw();
	};
	class 		InvalidRedirectionException: public std::exception
	{
		virtual const char* what() const throw();
	};
	/*class 		InvalidIndexException: public std::exception
	{
		virtual const char* what() const throw();
	};*/
	class 		InvalidAutoindexException: public std::exception
	{
		virtual const char* what() const throw();
	};
	class 		InvalidCgiExtensionException: public std::exception
	{
		virtual const char* what() const throw();
	};
	class 		InvalidCgiBinDirException: public std::exception
	{
		virtual const char* what() const throw();
	};
	class 		InvalidUploadDirException: public std::exception
	{
		virtual const char* what() const throw();
	};
};

#endif
