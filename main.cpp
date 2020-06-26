#include <iostream>
# include <fstream>
# include <sys/utsname.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <signal.h>
# include "daemon.hpp"

t_env		env;


int	kill_daemon()
{
  close(env.fd_file);
  close(env.sock);
  close(env.csock);
  unlink("/var/lock/matt_daemon.lock");
  rmdir("/var/lock");
  unlink("/var/log/matt_daemon/matt_daemon.log");
  rmdir("/var/log/matt_daemon");
  return (0);
}

void	handle(int sig)
{
  kill_daemon();
}


void	menu()
{ 
  dprintf(env.csock, "[       ?      ] => help\n");
  dprintf(env.csock, "[      os      ] => os name\n");
  dprintf(env.csock, "[     shell    ] => spawn bash in root\n");
  dprintf(env.csock, "[     quit     ] => quit Matt_daemon\n");
}


int	main()
{
  char			buf[512];
  char			av[] = {"-i"};
  int			r;
  struct utsname	unamee;
  FILE			*fp;
  int			rd;
  std::string		bin;
  
  signal(SIGINT, handle);
  if (!(access("/var/lock/matt_daemon.lock", F_OK)))
    std::cout << "Can't open: /var/lock/matt_daemon.lock" << std::endl;
  else
    if (!(rd = daemon(&env)))
      {
	std::cout << "Hello World [elbenkri]" << std::endl;
	std::ofstream f("/var/log/matt_daemon/matt_daemon.log");
	if (f)
	  {
	    r = 1;
	    uname(&unamee);
	    while (r > 0)
	      {
		dprintf(env.csock, "$>");
		r = read(env.csock, buf, 512);
		buf[r] = '\0';
		f << buf << std::endl;
		if (!strcmp(buf, "?\n"))
		    menu();
		else if (!strcmp(buf, "os\n"))
		    dprintf(env.csock, "uname is %s\n", unamee.sysname);
		else if (!strcmp(buf, "shell\n"))
		{
		  mkfifo("/tmp/tunn", 0644);
		  popen("cat /tmp/tunn|/bin/bash 2>&1|nc -l 4243 >/tmp/tunn", "w");
		  dprintf(env.csock, "shell spawning in port 4243\n");
		}
		else if (!strcmp(buf, "quit\n"))
		  break;
		memset(buf, 0, r);
	      }
	  }
	else
	  std::cout << "Error fostream" << std::endl;
      }
    else
      {
	if (rd != 1)
	  std::cout << "Error daemon: rd == " << rd << std::endl;
      }
  kill_daemon();
  return (0);
}
