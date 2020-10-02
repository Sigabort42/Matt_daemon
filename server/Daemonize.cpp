#include <iostream>
#include "Daemonize.hpp"
#include <unistd.h>

Daemonize::Daemonize(void) {
  std::cout << "Contructor Called" << std::endl;

  if (fork() > 0)
      std::cout << "Daemonize Lanched" << std::endl;
  else
    {
      std::cout << "Child" << std::endl;
      this->func_for_daemonize();
    }
  
  return ;
}

Daemonize::~Daemonize(void) {
  std::cout << "Destructor Called" << std::endl;
  return ;
}

void	Daemonize::port_global(int port) {
  Daemonize::_port_global = port;
  std::cout << "Port global added" << std::endl;
  return ;
}

int	Daemonize::get_port_global() {
  return Daemonize::_port_global;
}

int	Daemonize::_port_global = 4242;
