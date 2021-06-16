/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nessayan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 13:34:11 by nessayan          #+#    #+#             */
/*   Updated: 2021/06/15 13:34:25 by nessayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Route.hpp"

// CONSTRUCTOR & DESTRUCTOR

Route::Route(void) {
	this->path = std::string("default_path");
}

Route::Route(Route const &r) {
	this->path = r.path;
}

Route::~Route(void) {
}

// OPERATOR

Route				&Route::operator=(Route const &r) {
	this->path = r.path;
	return (*this);
}

// GETTERS

// SETTERS

void				Route::setPath(std::string const &field) {
	(void)field;
}

void				Route::setMethods(std::string const &field) {
	(void)field;
}

void				Route::setRedirection(std::string const &field) {
	(void)field;
}

void				Route::setAutoindex(std::string const &field) {
	(void)field;
}

void				Route::setCgiExtension(std::string const &field) {
	(void)field;
}

void				Route::setCgiBin(std::string const &field) {
	(void)field;
}

