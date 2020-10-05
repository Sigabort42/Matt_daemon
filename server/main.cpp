/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbenkri <elbenkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 00:52:20 by elbenkri          #+#    #+#             */
/*   Updated: 2020/06/30 00:52:22 by elbenkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/daemon.hpp"

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

void	call_tintin(int type, const std::string str)
{
      env.tr.setLog(str);
      env.f << env.tr.writeLog(type) << std::endl;
}

void	handle(int sig)
{
  std::string tmp = "Signal catched: " + std::to_string(sig);
  const char *buf = tmp.c_str();
  call_tintin(SIGNAL, buf);
}

void	handle_exit(int sig)
{
  kill_daemon();
  std::string tmp = "Signal catched: " + std::to_string(sig);
  const char *buf = tmp.c_str();
  call_tintin(SIGNAL, buf);
}

void	menu()
{
  std::string	msg;

  msg = "\
[       ?      ] => help\n\
[     shell    ] => spawn bash in root\n\
[     quit     ] => quit Matt_daemon\n";
  msg = encrypt(msg);
  write(env.csock, msg.c_str(), msg.length());
}

void	signal()
{
  signal(SIGHUP, handle);
  signal(SIGINT, handle);
  signal(SIGQUIT, handle);
  signal(SIGILL, handle);
  signal(SIGTRAP, handle);
  signal(SIGABRT, handle);
  signal(SIGEMT, handle);
  signal(SIGFPE, handle);
  signal(SIGBUS, handle);
  signal(SIGSEGV, handle_exit);
  signal(SIGSYS, handle);
  signal(SIGPIPE, handle_exit);
  signal(SIGALRM, handle);
  signal(SIGTERM, handle_exit);
  signal(SIGURG, handle);
  signal(SIGTSTP, handle);
  signal(SIGCHLD, handle);
  signal(SIGTTIN, handle);
  signal(SIGTTOU, handle);
  signal(SIGIO, handle);
  signal(SIGXCPU, handle);
  signal(SIGXFSZ, handle);
  signal(SIGVTALRM, handle);
  signal(SIGPROF, handle);
  signal(SIGWINCH, handle);
  signal(SIGINFO, handle);
  signal(SIGUSR1, handle);
  signal(SIGUSR2, handle);
}

int	main()
{
  char			buf[512];
  int			r;
  int			rd;
  int			pid;
  std::string		msg;
  std::string		lol;

  signal();
  if (getuid())
      std::cout << "Permission denied: execute as root" << std::endl;
  else if (!(access("/var/lock/matt_daemon.lock", F_OK)))
    {
      env.f.open("/var/log/matt_daemon/matt_daemon.log", std::fstream::app);
      call_tintin(FATAL, "Can't open: /var/lock/matt_daemon.lock");
      std::cout << "Can't open: /var/lock/matt_daemon.lock" << std::endl;
    }
  else
    if (!(rd = daemon(&env)))
      {
	if (env.f)
	  {
	    call_tintin(INFO, "Started Connexion");
	    while (msg.compare("Saluttoi"))
	      {
		msg = "Password:";
		msg = encrypt(msg);
		write(env.csock, msg.c_str(), msg.length());
		r = read(env.csock, buf, 512);
		msg = buf;
		if (r > 0)
		  {
		    buf[r] = '\0';
		    msg = decrypt(buf); 
		  }
		if (!msg.compare(""))
		  {
		    kill_daemon();
		    return (0);
		  }
		call_tintin(LOG, msg);
	      }
	    r = 1;
	    while (r > 0)
	      {
		memset(buf, 0, r);
		msg = "$>";
		msg = encrypt(msg);
		write(env.csock, msg.c_str(), msg.length());
		r = read(env.csock, buf, 512);
		msg = buf;
		if (r > 0)
		  {
		    buf[r] = '\0';
		    msg = decrypt(buf);
		  }
		if (!msg.compare(""))
		  {
		    kill_daemon();
		    return (0);
		  }
		call_tintin(LOG, msg);
		if (!msg.compare("?"))
		    menu();
		else if (!msg.compare("shell"))
		{
		  msg = "shell spawning in port 4243\n";
		  msg = encrypt(msg);
		  call_tintin(INFO, "Launch Shell on port 4243");
		  mkfifo("/tmp/tunn", 0644);
		  popen("cat /tmp/tunn|/bin/bash 2>&1|nc -l 4243 >/tmp/tunn", "r");
		  write(env.csock, msg.c_str(), msg.length());
		}
		else if (!msg.compare("quit"))
		  {
		    kill_daemon();
		    return (0);
		  }
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
