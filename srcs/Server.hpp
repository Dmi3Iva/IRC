#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h> // For sockaddr_in
#include <poll.h>
#include <string>
#include <sys/socket.h> // For socket functions
#include <unistd.h>
#include <vector>

#include "Context.hpp"
#include "utils.hpp"

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
public:
  static Server *getInstance();
  void start();
  ~Server();

private:
  int _socketfd;
  sockaddr_in _sockaddr;
  Context *_ctx;

  Server();
  Server(Server const &);         // Don't Implement.
  void operator=(Server const &); // Don't implement.

  void _acceptNewClients(pollfd *serverPollFd);
  void _startListening(int socketfd);
  void _bindSocket(string addr, int port);
  pair<string, string> _getConnectionInfo(int userFd);
};

#endif // IRC_SERVER_HPP
