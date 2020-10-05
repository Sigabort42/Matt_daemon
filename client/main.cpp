#include <iostream>
#include "../includes/daemon.hpp"

void	usage()
{
  std::cout << "BEN_AKF <host> <port>" << std::endl;
  exit(1);
}

int	create_client(char *host, int port)
{
  int			sock;
  struct protoent	*proto;
  struct sockaddr_in	sin;

  proto = getprotobyname("tcp");
  if (!proto)
    return (-1);
  sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = inet_addr(host);
  if (connect(sock, (const struct sockaddr*)&sin, sizeof(sin)) == -1)
    {
      std::cout << "Connect error" << std::endl;
      exit(2);
    }
  return (sock);
}
int	main(int ac, char **av)
{
  char			buf[512];
  std::string  		msg;
  int			port;
  int			sock;
  int			r;

  if (ac < 3)
    usage();
  port = atoi(av[2]);
  sock = create_client(av[1], port);
  r = 1;
  while (r > 0)
    {
      msg.clear();
      bzero(buf, 512);
      r = read(sock, buf, 512);
      buf[r] = '\0';
      msg = decrypt(buf);
      write(1 , msg.c_str(), msg.length());
      bzero(buf, 512);
      r = read(0, buf, 512);
      msg = buf;
      msg = "\n";
      if (r > 1)
	{
	  buf[r - 1] = '\0';
	  msg = encrypt(buf);
	}
      write(sock, msg.c_str(), msg.length());
      if (!strcmp(buf, "quit"))
	exit(0);
    }
  
  return (0);
}
