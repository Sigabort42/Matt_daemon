/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Daemonize.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbenkri <elbenkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 14:35:07 by elbenkri          #+#    #+#             */
/*   Updated: 2020/10/06 14:35:10 by elbenkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DAEMONIZE_HPP
# define DAEMONIZE_HPP
#include <iostream>

class Daemonize {

public:
  
  int		(*func_for_daemonize)(void);

  		Daemonize(void);
		Daemonize(int *func);
  virtual	~Daemonize(void);

  static void	port_global(int port);
  static int	get_port_global();

  Daemonize & operator=(Daemonize const &rhs);

private:
  static int	_port_global;
  
};

#endif
