/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 13:32:52 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/15 13:33:18 by nessayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
# define ROUTE_HPP

# include <string>
# include <vector>
# include <sstream>
# include <iostream>
# include <sys/stat.h>

class Route {
private:
	// ATTRIBUTES
	std::string					path;
	std::vector<std::string> 	methods;
	std::string					redirection;
	bool						autoindex;
	std::string					cgi_extension;
	std::string					cgi_bin;

	// PRIVATE HELPERS
	bool						isMethodDeclared(std::vector<std::string> methods, std::string word);

public:
	// CONSTRUCTOR & DESTRUCTOR
	Route(void);
	Route(Route const &r);
	~Route(void);

	// OPERATOR
	Route						&operator=(Route const &r);

	// GETTERS
	std::string					getPath(void) const;
	std::vector<std::string>	getMethods(void) const;
	std::string					getRedirection(void) const;
	bool						getAutoindex(void) const;
	std::string					getCgiExtension(void) const;
	std::string					getCgiBin(void) const;

	// SETTERS
	void						setPath(std::string const &field);
	void						setMethods(std::string const &field);
	void						setRedirection(std::string const &root, std::string const &field);
	void						setAutoindex(std::string const &field);
	void						setCgiExtension(std::string const &field);
	void						setCgiBin(std::string const &field);
};

#endif
