#include "../includes/daemon.hpp"

std::string	encrypt(std::string buf)
{
  std::string	msg;
  int		i;

  i = 0;
  while (buf[i])
    msg += buf[i++] + 13;
  return (msg);
}

std::string	decrypt(std::string buf)
{
  std::string	msg;
  int		i;

  i = 0;
  while (buf[i])
    msg += buf[i++] - 13;
  return (msg);
}
