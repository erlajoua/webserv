/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 13:32:01 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/15 13:32:45 by nessayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <vector>

# include "./Route.hpp"

class Server {
private:
	// ATTRIBUTES
	//int 						port; 
	std::string 				host;
	std::string 				server_name;
	std::string					root;
	std::vector<std::string>	errors;
	//int 						client_body_size;
	std::string					upload_dir;	
	std::vector<Route> 			routes;

public:
	// CONSTRUCTOR & DESTRUCTOR
	Server(void);
	Server(Server const &s);
	~Server(void);

	// OPERATOR
	Server						&operator=(Server const &s);

	// GETTERS
	std::string					getServerName(void) const;
	std::vector<Route>			getRoutes(void) const;

	// SETTERS
	void						setPort(std::string const &field);
	void						setHost(std::string const &field);
	void						setServerName(std::string const &field);
	void						setRoot(std::string const &field);
	void						setErrors(std::string const &field);
	void						setClientBodySize(std::string const &field);
	void						setUploadDir(std::string const &field);	
};

#endif
