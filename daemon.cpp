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

# include "daemon.hpp"
# define PORT 4242


int			create_env_work(t_env *env)
{
  if ((mkdir("/var/lock", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == -1)
    return (-2);
  if ((creat("/var/lock/matt_daemon.lock", S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
    return (-1);
  if ((mkdir("/var/log/matt_daemon", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == -1)
    //    return (-1);
    ;
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
  bind(sock, (const struct sockaddr*)&sin, sizeof(sin));
  listen(sock, 3);
  return (sock);
}

int			persiste_os(t_env *env)
{
  uname(&env->unamee);
  if (!(strcmp(env->unamee.sysname, "Darwin")))
    {
      if ((persiste_darwin(env)) == 1)
	call_tintin(ERROR, "File persistence exist");
      else if ((persiste_darwin(env)) == -1)
	call_tintin(ERROR, "Not created persistence");
    }
  else if (!(strcmp(env->unamee.sysname, "Linux")))
    {
      if ((persiste_linux(env)) == 1)
	call_tintin(ERROR, "File persistence exist");
      else if ((persiste_linux(env)) == -1)
	call_tintin(ERROR, "Not created persistence");
    }
  return (0);
}

int			daemon(t_env *env)
{
  struct sockaddr_in	csin;
  unsigned int		cslen;


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
      //      persiste_os(env);
      call_tintin(INFO, "Daemonize Successful");
      env->csock = accept(env->sock, (struct sockaddr*)&csin, &cslen);
      return (0);
    }
  if (env->pid)
    exit(0);
  return (1);
}
