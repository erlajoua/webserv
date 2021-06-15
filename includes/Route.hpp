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
	std::vector<std::string> 		methods;
	std::string						redirection;
	bool							autoindex;
	std::string						cgi_extension;
	std::string						cgi_bin;

	// UNUSED NORMALIZED FUNCTIONS
	Route(Route const &r);
	Route		&operator=(Route const &r);

public:
	Route(void);
	~Route(void);
};

#endif
