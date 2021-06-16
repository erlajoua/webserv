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

class Route {
private:
	// ATTRIBUTES
	std::string					path;
	std::vector<std::string> 	methods;
	std::string					redirection;
	bool						autoindex;
	std::string					cgi_extension;
	std::string					cgi_bin;

public:
	// CONSTRUCTOR & DESTRUCTOR
	Route(void);
	Route(Route const &r);
	~Route(void);

	// OPERATOR
	Route						&operator=(Route const &r);

	// GETTERS

	// SETTERS
	void						setPath(std::string const &field);
	void						setMethods(std::string const &field);
	void						setRedirection(std::string const &field);
	void						setAutoindex(std::string const &field);
	void						setCgiExtension(std::string const &field);
	void						setCgiBin(std::string const &field);
};

#endif
