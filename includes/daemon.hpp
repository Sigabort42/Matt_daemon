/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbenkri <elbenkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 00:52:12 by elbenkri          #+#    #+#             */
/*   Updated: 2020/06/30 00:52:15 by elbenkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DAEMON_HPP
# define DAEMON_HPP
# define PORT 4242
# define MAX_CLIENTS 3
# include <iostream>
# include <fstream>
# include <thread>
# include <sys/utsname.h>
# include <sys/stat.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <signal.h>
# include <errno.h>
# include "Tintin_Reporter.hpp"

struct			t_sock
{
  int			sock;
  int			is_connect;
};

struct			t_env
{
  pid_t			ppid;
  pid_t			pid;
  pid_t			sid;
  int			fd_file;
  int			sock;
  int			nfds;
  int			i;
  int			total_client;
  t_sock		csock[MAX_CLIENTS];
  struct utsname        unamee;
  std::fstream		f;
  Tintin_Reporter	tr;
};


enum {
      INFO,
      LOG,
      ERROR,
      FATAL,
      SIGNAL,
};

void		call_tintin(int type, std::string str);
void		search_i_in_csock(std::string, int r, char *buf);
std::string	encrypt(std::string buf);
std::string	decrypt(std::string buf);
int		create_env_work(t_env *env);
int		persiste_darwin(t_env *env);
int		persiste_linux(t_env *env);
int		daemon(t_env *env);
int		kill_daemon();
int		prompt(std::string msg, int r, char *buf);
#endif
