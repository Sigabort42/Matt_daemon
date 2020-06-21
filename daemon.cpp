#include <unistd.h>
#include <stdlib.h>
# include <iostream>

int daemon()
{
  pid_t		pid;
  pid_t		ppid;
  pid_t		sid;

  if ((pid = fork()) == -1)
    return (-1);
  if (!pid)
    {
      sid = setsid();
      if (!getuid())
	while (1);
      else
	std::cout << "Not root" << std::endl;
    }
  if (pid)
    exit(0);
  
  return (1);
} 
