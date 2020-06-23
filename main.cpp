#include <iostream>
# include <fstream>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <signal.h>
# include "daemon.hpp"

t_env		env;


int	kill_daemon(t_env *env)
{
  close(env->fd_file);
  close(env->sock);
  close(env->csock);
  unlink("/var/lock/matt_daemon.lock");
  rmdir("/var/lock");
  //  unlink("/var/log/matt_daemon/matt_daemon.log");
  //rmdir("/var/log/matt_daemon");
  return (0);
}

void	handle(int sig)
{
  kill_daemon(&env);
}

int	main()
{
  char		buf[512];
  char		av[] = {"-i"};
  int		r;
  FILE		*fp;

  signal(SIGINT, handle);
  if (!(access("/var/lock/matt_daemon.lock", F_OK)))
    std::cout << "Error open file /var/lock/matt_daemon.lock" << std::endl;
  else
    if (!(daemon(&env)))
      {
	std::cout << "OKOKOK" << std::endl;
	std::ofstream f("/var/log/matt_daemon/matt_daemon.log");
	if (f)
	  {
	    while ((r = read(env.csock, buf, 512)) > 0)
	      {
		buf[r] = '\0';
		f << buf << std::endl;
		if (!strcmp(buf, "shell\n"))
		  {
		    std::cout << "ca va venir" << std::endl;
		    //		fp = popen("/bin/sh -i", "w");
		  }
		if (!strcmp(buf, "quit\n"))
		  break;
		memset(buf, 0, r);
	      }
	  }
	else
	  std::cout << "Error flux" << std::endl;
	kill_daemon(&env);
      }
  return (0);
}
