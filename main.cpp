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
  call_tintin(INFO, "Quit Daemon");
  close(env.fd_file);
  close(env.sock);
  close(env.csock);
  call_tintin(INFO, "Close Connexion Daemon port 4242");
  unlink("/var/lock/matt_daemon.lock");
  rmdir("/var/lock");
  call_tintin(INFO, "free file /var/lock/matt_daemon.lock");
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

void	call_tintin(int type, const char *str)
{
      env.tr.setLog(str);
      env.f << env.tr.writeLog(type) << std::endl;
}

int	main()
{
  char			buf[512];
  int			r;
  struct utsname	unamee;
  int			rd;
  std::string lol;

  signal(SIGINT, handle);
  if (getuid())
      std::cout << "Not root" << std::endl;
  else if (!(access("/var/lock/matt_daemon.lock", F_OK)))
    {
      env.f.open("/var/log/matt_daemon/matt_daemon.log", std::fstream::app);
      call_tintin(FATAL, "Can't open: /var/lock/matt_daemon.lock");
      std::cout << "Can't open: /var/lock/matt_daemon.lock" << std::endl;
    }
  else
    if (!(rd = daemon(&env)))
      {
	//	std::cout << "Hello World [elbenkri]" << std::endl;
	if (env.f)
	  {
	    call_tintin(INFO, "Started Connexion");
	    r = 1;
	    uname(&unamee);
	    while (r > 0)
	      {
		dprintf(env.csock, "$>");
		r = read(env.csock, buf, 512);
		buf[r - 1] = '\0';
		call_tintin(LOG, buf);
		if (!strcmp(buf, "?"))
		    menu();
		else if (!strcmp(buf, "os"))
		    dprintf(env.csock, "uname is %s\n", unamee.sysname);
		else if (!strcmp(buf, "shell"))
		{
		  call_tintin(INFO, "Launch Shell on port 4243");
		  mkfifo("/tmp/tunn", 0644);
		  popen("cat /tmp/tunn|/bin/bash 2>&1|nc -l 4243 >/tmp/tunn", "w");
		  dprintf(env.csock, "shell spawning in port 4243\n");
		}
		else if (!strcmp(buf, "quit"))
		  {
		    kill_daemon();
		    break;
		  }
		memset(buf, 0, r);
	      }
	  }
	else
	  call_tintin(ERROR, "Error fostream");
      }
    else
      {
	if (rd != 1)
	  call_tintin(ERROR, "Error create daemon");
	std::cout << "Error daemon: rd == " << rd << std::endl;
      }
  return (0);
}
