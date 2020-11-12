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
fd_set		read_fd;
fd_set		write_fd;
fd_set		active_fd;

void	auth(std::string msg, int r, char *buf)
{

  call_tintin(INFO, "Connect Authentification");
  r = read(env.csock[env.i].sock, buf, 512);
  if (r <= 0)
    {
      if (r < 0)
	call_tintin(FATAL, "read prompt");
      else
	{
	  close(env.csock[env.i].sock);
	  env.csock[env.i].is_connect = 0;
	  FD_CLR(env.csock[env.i].sock, &active_fd);
	  env.total_client--;
	  if (env.total_client <= 0)
	    kill_daemon();
	}
      return ;
    }

  msg = buf;
  if (r > 0)
    {
      buf[r] = '\0';
      msg = decrypt(buf);
      msg.erase(msg.size() - 1);
      call_tintin(LOG, msg.c_str());
      if (!msg.compare("Saluttoi"))
	{
	  call_tintin(INFO, "Started Connexion");
	  env.csock[env.i].is_connect = 1;
	  return ;
	}
    }
  msg = "Password:";
  msg = encrypt(msg);
  write(env.csock[env.i].sock, msg.c_str(), msg.length());
}

int	kill_daemon()
{
  if (env.total_client >= 1)
    {
      call_tintin(INFO, "Connexion close");
      return (0);
    }
  call_tintin(INFO, "Quit Daemon");
  close(env.fd_file);
  close(env.sock);
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
  call_tintin(SIGNAL, tmp.c_str());
}

void	handle_exit(int sig)
{
  kill_daemon();
  std::string tmp = "Signal catched: " + std::to_string(sig);
  const char *buf = tmp.c_str();
  call_tintin(SIGNAL, buf);
  exit(sig);
}

void	menu()
{
  std::string	msg;

  msg = "\
[       ?      ] => help\n\
[     shell    ] => spawn bash in root\n\
[     quit     ] => quit Matt_daemon\n$>";
  msg = encrypt(msg);
  write(env.csock[env.i].sock, msg.c_str(), msg.length());
}

void	signal()
{
  signal(SIGHUP, handle);
  signal(SIGINT, handle);
  signal(SIGQUIT, handle);
  signal(SIGILL, handle);
  signal(SIGTRAP, handle);
  signal(SIGABRT, handle);
  signal(SIGIOT, handle);
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
  signal(SIGIO, handle);
  signal(SIGUSR1, handle);
  signal(SIGUSR2, handle);
}

int			listen_multi_sock()
{
  std::string		msg;
  char			buf[512];
  int			r;
  
  read_fd = active_fd;
  if (select(FD_SETSIZE, &read_fd, NULL, NULL, NULL) < 0)
    {
      call_tintin(FATAL, "select");
      exit(5);
    }
  search_i_in_csock(msg, r, buf);
  return (env.csock[env.i].sock);
}

void			search_i_in_csock(std::string msg, int r, char *buf)
{
  unsigned int          cslen;
  struct sockaddr_in    csin;
  int			i;
  int			closefd;
  
  env.i = 0;
  i = 0;
  while (i < FD_SETSIZE)
    {
      if (FD_ISSET(i, &read_fd))
	{
	  if (i == env.sock)
	    {
	      if (env.total_client >= MAX_CLIENTS)
		{
		  closefd  = accept(env.sock, (struct sockaddr*)&csin, &cslen);
		  close(closefd);
		  return ; 
		}
	      env.nfds = accept(env.sock, (struct sockaddr*)&csin, &cslen);
	      if (env.nfds < 0)
		{
		  call_tintin(FATAL, "accept");
		  exit(5);
		}
	      env.csock[env.total_client].is_connect = 0;
	      env.csock[env.total_client].sock = env.nfds;
	      FD_SET(env.nfds, &active_fd);
	      env.total_client++;
	    }
	  else
	    {
	      while (env.i < MAX_CLIENTS && env.csock[env.i].sock != i)
		env.i++;
	      if (i == env.csock[env.i].sock)
		{
		  if (env.csock[env.i].is_connect)
		    prompt(msg, r, buf);
		  else
		    auth(msg, r, buf);
		}
	    }
	}
      i++;
    }
}

int	prompt(std::string msg, int r, char *buf)
{
	msg.clear();
	r = read(env.csock[env.i].sock, buf, 512);
	errno = 0;
	if (r <= 0)
	{
		if (r < 0)
			call_tintin(FATAL, "read prompt");
		else
		{
			call_tintin(INFO, "close clientt %d\n");
			close(env.csock[env.i].sock);
			env.csock[env.i].is_connect = 0;
			FD_CLR(env.csock[env.i].sock, &active_fd);
			env.total_client--;
			if (env.total_client <= 0)
				kill_daemon();
		}
		return (1);
	}
	msg = buf;
	if (r > 0)
	{
		buf[r - 1] = '\0';
		msg = decrypt(buf);
	}
	call_tintin(LOG, msg);
	if (!msg.compare("?"))
		menu();
	else if (!msg.compare("shell"))
	{
		msg = "shell spawning in port 4243\n$>";
		msg = encrypt(msg);
		call_tintin(INFO, "Launch Shell on port 4243");
		if (mkfifo("/tmp/tunn", 0644) < 0 && errno != EEXIST)
		{
			msg = encrypt("Shell is not working");
			write(env.csock[env.i].sock, msg.c_str(), msg.length());
		}
		else
		{
			popen("cat /tmp/tunn|/bin/bash 2>&1|nc -l 4243 >/tmp/tunn", "r");
			write(env.csock[env.i].sock, msg.c_str(), msg.length());	      
		}
	}
	else if (!msg.compare("quit"))
	{
		kill_daemon();
		return (1);
	}
	else
	{
		memset(buf, 0, r);
		msg = "$>";
		msg = encrypt(msg);
		write(env.csock[env.i].sock, msg.c_str(), msg.length());
	}
	return (0);
}


int	main()
{
  char			buf[512];
  int			rd;
  std::string		msg;
  
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
	    FD_ZERO(&active_fd);
	    FD_SET(env.sock, &active_fd);
	    env.csock[0].sock = -1;
	    env.csock[1].sock = -1;
	    env.csock[2].sock = -1;
	    env.total_client = 0;
	    while (1)
		listen_multi_sock();
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
