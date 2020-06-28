#ifndef DAEMON_HPP
# define DAEMON_HPP
# include <iostream>
# include <fstream>
# include "Tintin_Reporter.hpp"

struct t_env
{
  pid_t			ppid;
  pid_t			pid;
  pid_t			sid;
  int			fd_file;
  int			sock;
  int			csock;
  std::fstream		f;
  Tintin_Reporter	tr;
};


enum {
      INFO,
      LOG,
      ERROR,
      FATAL,
      SIGNAL,
};

void	call_tintin(int type, const char *str);
int	create_env_work(t_env *env);
int	daemon(t_env *env);

#endif
