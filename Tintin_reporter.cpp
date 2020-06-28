#include <iostream>
# include <time.h>
# include <string>
# include "daemon.hpp"
# include "Tintin_Reporter.hpp"

Tintin_Reporter::Tintin_Reporter(void) {
  //  std::cout << "Default Constructor Called" << std::endl;
  return ;
}

Tintin_Reporter::Tintin_Reporter(Tintin_Reporter const &src) {
  //  std::cout << "Copy Constructor Called" << std::endl;
  *this = src;
  return ;
}

Tintin_Reporter::~Tintin_Reporter(void) {
  //  std::cout << "Destructor Called" << std::endl;
  return ;
}

std::string	Tintin_Reporter::getLog(void) const {
  return this->_log;
}

void		Tintin_Reporter::setLog(std::string log) {
  this->_log = log;
  return ;
}

std::string	Tintin_Reporter::writeLog(int type) {
  std::string	ret;
  std::string	date;
  time_t	now;
  tm		*ltm;
  
  time(&now);
  ltm = localtime(&now);
  date = std::to_string(ltm->tm_mday) + "/"
    + std::to_string(1 + ltm->tm_mon) + "/"
    + std::to_string(1900 + ltm->tm_year) + " "
    + std::to_string(ltm->tm_hour) + ":"
    + std::to_string(ltm->tm_min) + ":"
    + std::to_string(ltm->tm_sec);
  ret = "[" + date + "] - ";
  if (type == ERROR)
    return (ret + "[ERROR]  - " + this->getLog());
  else if (type == INFO)
    return (ret + "[INFO]   - " + this->getLog());
  else if (type == LOG)
    return (ret + "[LOG]    - User input: " + this->getLog());
  else if (type == FATAL)
    return (ret + "[FATAL]  - " + this->getLog());
  else if (type == SIGNAL)
    return (ret + "[SIGNAL] - " + this->getLog());
  return (0);
}

Tintin_Reporter & Tintin_Reporter::operator=(Tintin_Reporter const &rhs) {
  //  std::cout << "Opertor Assignation Called" << std::endl;
  this->_log = rhs.getLog();
  return *this;
}
