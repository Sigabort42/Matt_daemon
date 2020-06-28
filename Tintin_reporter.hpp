#ifndef TINTIN_REPORTER_H
# define TINTIN_REPORTER_H
# include <iostream>

class Tintin_Reporter {

public:

		Tintin_Reporter(void);
  		Tintin_Reporter(Tintin_Reporter const & src);
  virtual	~Tintin_Reporter(void);


  std::string	writeLog(int type);
  std::string	getLog(void) const;
  void		setLog(std::string log);
  
  Tintin_Reporter & operator=(Tintin_Reporter const &rhs);

protected:
  std::string	_log;

  
};

//std::ostream & operator<<(std::ostream o, )
  
#endif
