#include <unistd.h>
# include <stdlib.h>
# include <iostream>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "daemon.hpp"
# define PORT 4242


int			create_env_work(t_env *env)
{
  if ((mkdir("/var/lock", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == -1)
    //return (-2);
    ;
  if ((mkdir("/var/log/matt_daemon", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == -1)
    return (-1);
  if ((creat("/var/lock/matt_daemon.lock", S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
    return (-1);
  env->fd_file = creat("/var/log/matt_daemon/matt_daemon.log", S_IRWXU | S_IRWXG | S_IRWXO);
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

int			daemon(t_env *env)
{
  struct sockaddr_in	csin;
  unsigned int		cslen;
  
  if ((env->pid = fork()) == -1)
    return (-1);
  if (!env->pid)
    {
      if ((env->sid = setsid()) == -1)
	return (-2);
      if (!getuid())
	{
	  if ((env->sock = create_server()) == -1)
	    return (-3);
	  else if (!(create_env_work(env)))
	    env->csock = accept(env->sock, (struct sockaddr*)&csin, &cslen);
	  else
	    return (-4);
//	  std::cout << "lololol=>>> " << csin.sin_addr.s_addr << std::endl;
	  return (0);
	}
      else
	std::cout << "Not root" << std::endl;
    }
  if (env->pid)
    exit(0);  
  return (1);
} 
