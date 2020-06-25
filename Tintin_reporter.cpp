#include <iostram>
# include "Tintin_Reporter.hpp"

Tintin_Reporter::Tintin_Reporter(std::string signal) {
  std::cout << "Constructor Called" std::endl;
  return ;
}

Tintin_Reporter::~Tintin_Reporter(void) {

  std::cout << "Destructor Called" std::endl;
  return ;
}

