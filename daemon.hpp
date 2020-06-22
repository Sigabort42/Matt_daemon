#ifndef DAEMON_HPP
# define DAEMON_HPP

typedef struct	s_env
{
  pid_t		ppid;
  pid_t		pid;
  pid_t		sid;
  int		fd_file;
  int		sock;
  int		csock;
}		t_env;

int	daemon(t_env *env);

#endif
