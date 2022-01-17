//
// Created by Twycross Redfort on 1/17/22.
//

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <sys/fcntl.h>
# include <netinet/in.h>
#include <unistd.h>
# include <cstdio>


class Socket {

private:
  struct sockaddr_in _address;
  int _sockfd;
  int _backlog;

public:
  Socket(int, int, int, int, in_addr_t, int);
  ~Socket();

  const sockaddr_in& getAddress() const;
  int getSockfd() const;
  int getBacklog() const;

  void allowReuseAddress();
  void setNonblockMode();
  void bindSocket();
  void startListening();

private:
  Socket();
  Socket(const Socket&);
  Socket& operator=(const Socket&);

  void testConnection(int);

};

#endif
