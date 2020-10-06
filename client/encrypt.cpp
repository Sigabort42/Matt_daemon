/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbenkri <elbenkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 14:34:44 by elbenkri          #+#    #+#             */
/*   Updated: 2020/10/06 14:34:47 by elbenkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/daemon.hpp"

std::string	encrypt(std::string buf)
{
  std::string	msg;
  int		i;

  i = 0;
  while (buf[i])
    msg += buf[i++] + 13;
  return (msg);
}

std::string	decrypt(std::string buf)
{
  std::string	msg;
  int		i;

  i = 0;
  while (buf[i])
    msg += buf[i++] - 13;
  return (msg);
}
