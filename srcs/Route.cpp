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

Route::Route(void) {
}

Route::Route(Route const &r) {
	(void)r;
}

Route::~Route(void) {
}

Route				&Route::operator=(Route const &r) {
	(void)r;
	return (*this);
}
