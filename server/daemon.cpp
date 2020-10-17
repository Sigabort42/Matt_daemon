/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbenkri <elbenkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 00:52:03 by elbenkri          #+#    #+#             */
/*   Updated: 2020/06/30 00:52:06 by elbenkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/daemon.hpp"

int			create_env_work(t_env *env)
{
  errno = 0;
  if ((mkdir("/var/lock", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == -1)
      if (errno != EEXIST)
	return (-2);
  if ((creat("/var/lock/matt_daemon.lock", S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
    return (-1);
  if ((mkdir("/var/log/matt_daemon", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == -1)
    if (errno != EEXIST)
	return (-1);
  env->f.open("/var/log/matt_daemon/matt_daemon.log", std::fstream::app);
  return (0);
}

int			create_server()
{
  struct protoent	*proto;
  struct sockaddr_in	sin;
  int			sock;
  
  proto = getprotobyname("tcp");
  if (!proto)
    return (-1);
  sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(PORT);
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sock, (const struct sockaddr*)&sin, sizeof(sin)) < 0)
    {
      perror("bind");
      kill_daemon();
      exit(10);
    }
  listen(sock, 1);
  return (sock);
}

int			daemon(t_env *env)
{
  call_tintin(INFO, "Started");
  if ((create_env_work(env)))
    {
      call_tintin(ERROR, "Error Started");
      return (-4);
    }
  call_tintin(INFO, "Create Server");
  if ((env->sock = create_server()) == -1)
    {
      call_tintin(ERROR, "Error Create Server");
      return (-3);
    }
  call_tintin(INFO, "Server Created");
  call_tintin(INFO, "Entering for Daemonize");
  if ((env->pid = fork()) == -1)
    {
      call_tintin(ERROR, "Error Daemonize");
      return (-1);
    }
  if (!env->pid)
    {
      if ((env->sid = setsid()) == -1)
	{
	  call_tintin(ERROR, "Error Daemonize");
	  return (-2);
	}
      return (0);
    }
  if (env->pid)
    {
      call_tintin(INFO, "Daemonize Successful - PID : " + std::to_string(env->pid));
      exit(0);
    }
  return (1);
}
