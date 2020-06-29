#include "daemon.hpp"

int	persiste_darwin(t_env *env)
{
  std::string	plist;
  std::fstream	f;

  if (!(access("/Library/LaunchDaemons/com.matt.daemon.plist", F_OK)))
    if (!(access("/usr/local/bin/Matt_daemon", X_OK)))
      return (1);
  
  plist = "<?xml version='1.0' encoding='UTF-8'?>\n<!DOCTYPE plist PUBLIC '-//Apple//DTD PLIST 1.0//EN' 'http://www.apple.com/DTDs/PropertyList-1.0.dtd'>\n<plist version='1.0'>\n<dict>\n<key>Label</key>\n<string>com.matt.daemon</string>\n<key>Program</key>\n<string>/usr/local/bin/Matt_daemon</string>\n<key>RunAtLoad</key>\n<true/>\n</dict>\n</plist>";

  if ((creat("/Library/LaunchDaemons/com.matt.daemon.plist", S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
    return (-1);
  f.open("/Library/LaunchDaemons/com.matt.daemon.plist", std::fstream::app);

  popen("cp Matt_daemon /usr/local/bin/Matt_daemon", "w");
  f << plist << std::endl;
  return (0);
}

int	persiste_linux(t_env *env)
{
  return (0);
}
