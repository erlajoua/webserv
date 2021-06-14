/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Program.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 14:46:51 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/14 16:54:19 by nessayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROGRAM_HPP
# define PROGRAM_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <stdexcept>

class Program {
private:
	Program(Program const &p);
	Program		&operator=(Program const &p);
	int			checkErrorConfig(void);
public:
	Program(void);
	~Program(void);
	void		parseConfig(std::string path);
	void		start(void);
	void		stop(void);
	class 		IncorrectConfigFileException: public std::exception {
		virtual const char* what() const throw();
	};
};

#endif
