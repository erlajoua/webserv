/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 11:31:27 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/22 11:31:30 by nessayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include "./Response.hpp"
# include <dirent.h>

class AutoIndex
{
	private:
		// ATTRIBUTES
		std::string						location_path;
		std::string						root;
		std::vector<std::string>		filenames;
		std::vector<std::string>		entries;
		std::string						index;

		// PRIVATE HELPERS

		void				getFilenames(void);
		static bool			compareFilenames(std::string const &entry_a, std::string const &entry_b);
		bool 				isDirectory(std::string const &name);
		void				processFilenames(void);
		void				createEntries(void);
		std::string			getLastModified(std::string const &path);
		void				addIndexLine(int line);

		// NOT USED
		AutoIndex(void);
		AutoIndex(AutoIndex const &a);
		AutoIndex 			&operator=(AutoIndex const &a);

	public:
		// CONSTRUCTOR & DESTRUCTOR
		AutoIndex(std::string const &root, std::string const &location_path);
		~AutoIndex(void);

		// MEMBER FUNCTIONS
		void				createIndex(void);

		// GETTERS
		std::string	const 	&getIndex(void) const;
};

#endif