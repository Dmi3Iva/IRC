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
  int _sockfd;
  int _backlog;

public:
  Socket(int, int, int);
  ~Socket();

  int getSockfd() const;
  int getBacklog() const;

  void setAddressReuseMode();
  void setNonblockMode();
  void bindToAddress(struct sockaddr*);
  void startListening(int);

private:
  Socket();
  Socket(const Socket&);
  Socket& operator=(const Socket&);

  void testConnection(int);

};

#endif
