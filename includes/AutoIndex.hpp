/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 11:31:27 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/22 20:37:12 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include "Response.hpp"
# include <dirent.h>
# include <algorithm>

class AutoIndex
{
	private:
		// ATTRIBUTES
		std::string						location_path;
		std::string						root;
		std::vector<std::string>		files;
		std::vector<std::string>		adj_files;
		std::string						page_content;

		// PRIVATE HELPERS

		void				parseFiles(void);
		static bool			sortHelper(std::string const &file1, std::string const &file2);
		bool 				isDir(std::string const &name);
		void				processFiles(void);
		void				adjustFiles(void);
		std::string			getLastModified(std::string const &path);
		void				addContentLine(int line);

		// NOT USED
		AutoIndex(void);
		AutoIndex(AutoIndex const &a);
		AutoIndex 			&operator=(AutoIndex const &a);

	public:
		// CONSTRUCTOR & DESTRUCTOR
		AutoIndex(std::string const &root, std::string const &location_path);
		~AutoIndex(void);

		// MEMBER FUNCTIONS
		void				buildAutoIndex(void);

		// GETTERS
		std::string	const 	&getPageContent(void) const;
};

#endif
