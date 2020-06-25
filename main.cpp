#include <iostream>
# include <fstream>
# include <sys/utsname.h>
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
  dprintf(env.csock, "[     shell    ] => spawn bash\n");
  dprintf(env.csock, "[ i ncat linux ] => Install netcat for span shell in Debian\n");
  dprintf(env.csock, "[  i ncat osx  ] => Install netcat for span shell in Mac Osx\n");
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
    std::cout << "Error open file /var/lock/matt_daemon.lock" << std::endl;
  else
    if (!(rd = daemon(&env)))
      {
	std::cout << "OKOKOK" << std::endl;
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
		else if (!strcmp(buf, "i ncat linux\n"))
		{
		  system("apt-get install -y nmap >/dev/null");
		}
		else if (!strcmp(buf, "i ncat osx\n"))
		  {
		    system("/bin/bash -c '$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)' >/dev/null");
		    system("brew install nmap >/dev/null");
		  }
		else if (!strcmp(buf, "shell\n"))
		{
		  if (!strcmp(unamee.sysname, "Darwin"))
		    fp = popen("/usr/local/bin/ncat -l -e /bin/bash 4243", "w");
		  else if (!strcmp(unamee.sysname, "Linux"))
		    fp = popen("/usr/bin/ncat -l -e /bin/bash 4243", "w");
		  dprintf(env.csock, "shell spawning in port 4243\n");
		}
		else if (!strcmp(buf, "quit\n"))
		  break;
		memset(buf, 0, r);
	      }
	  }
	else
	  std::cout << "Error fostream" << std::endl;
	kill_daemon();
      }
    else
      {
	if (rd != 1)
	  std::cout << "Error daemon: rd == " << rd << std::endl;
      }
  return (0);
}
