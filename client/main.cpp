/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbenkri <elbenkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 14:34:54 by elbenkri          #+#    #+#             */
/*   Updated: 2020/10/06 14:34:56 by elbenkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../includes/daemon.hpp"

void	usage()
{
  std::cout << "BEN_AKF <host> <port>" << std::endl;
  exit(1);
}

int	create_client(char *host, int port)
{
  int			sock;
  struct protoent	*proto;
  struct sockaddr_in	sin;

  proto = getprotobyname("tcp");
  if (!proto)
    return (-1);
  sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = inet_addr(host);
  if (connect(sock, (const struct sockaddr*)&sin, sizeof(sin)) == -1)
    {
      std::cout << "Connect error" << std::endl;
      exit(2);
    }
  return (sock);
}

void	auth(int sock, char *buf, std::string msg, int r)
{
  msg = "\n";
  while (1)
    {
      write(sock, msg.c_str(), msg.length());
      msg.clear();
      bzero(buf, 512);
      r = read(sock, buf, 512);
      buf[r] = '\0';
      msg = decrypt(buf);
      write(1 , msg.c_str(), msg.length());
      bzero(buf, 512);
      r = read(0, buf, 512);
      if (r > 1)
	{
	  buf[r] = '\0';
	  msg = encrypt(buf);
	  if (!strcmp(buf, "Saluttoi\n"))
	    {
	      write(sock, msg.c_str(), msg.length());
	      return ;
	    }
	  if (!strcmp(buf, "quit\n"))
	    exit(0);
	}
    }  
}

void	comm(int sock, char *buf, std::string msg, int r)
{
  sleep(1);
  msg = "\n";
  while (1)
    {
      write(sock, msg.c_str(), msg.length());
      msg.clear();
      bzero(buf, 512);
      r = read(sock, buf, 512);
      buf[r] = '\0';
      msg = decrypt(buf);
      write(1 , msg.c_str(), msg.length());
      bzero(buf, 512);
      r = read(0, buf, 512);
      if (r > 1)
	{
	  buf[r] = '\0';
	  if (!strcmp(buf, "quit"))
	    exit(0);
	  msg = encrypt(buf);
	}
    }  
}

int	main(int ac, char **av)
{
  char			buf[512];
  std::string  		msg;
  int			port;
  int			sock;
  int			r;
  
  if (ac < 3)
    usage();
  port = atoi(av[2]);
  sock = create_client(av[1], port);
  r = 1;
  auth(sock, buf, msg, r);
  printf("Conncted\n");
  comm(sock, buf, msg, r);
  return (0);
}
