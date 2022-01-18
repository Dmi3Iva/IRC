#ifndef SERVER_HPP
#define SERVER_HPP
//#include <arpa/inet.h>
//#include <cerrno>
//#include <cstdlib> // For exit() and EXIT_FAILURE
//#include <fcntl.h>
//#include <netdb.h>
//#include <netinet/in.h> // For sockaddr_in
//#include <poll.h>
//#include <string>
//#include <sys/socket.h> // For socket functions
//#include <unistd.h>
//#include <vector>

# include <iostream>
# include "Socket.hpp"
# include "../Context.hpp"
# include "../utils.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Context;

/**
 * Starting IRC server,
 * checks for new connections,
 * creates Context which execute commands and contains info about channels and users
 */
class Server {

private:
  typedef vector<struct pollfd> pollfdType;
  Socket* _socket;
  Context* _сontext;
  string _password;
  pollfdType _pollfds;

public:
  Server(in_addr_t, int, string);
  ~Server();

  void start();
  void polling();
  void acceptNewClients();
  void closeConnections(int);
  bool receiveRequest();
  bool toSendResponse();

private:
  Server(Server const &);         // Don't Implement.
  void operator=(Server const &); // Don't implement.

  void _printConnectionInfo(int userFd);
};

#endif
