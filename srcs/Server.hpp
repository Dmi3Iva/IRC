#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP
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
#include "User.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Context;

class Server {
public:
  static Server *getInstance();
  pollfd *getPollFdFromFd(int fd);
  void start();
  ~Server();

private:
  int _socketfd;
  sockaddr_in _sockaddr;
  // TODO:: move to Context
  std::vector<User> Users;
  int _stopFlag;
  Context *_ctx;

  Server();
  Server(Server const &);         // Don't Implement.
  void operator=(Server const &); // Don't implement

  void _checkUsers();
  void _acceptNewClients(pollfd *serverPollFd);
  void _startListening(int socketfd);
  void _init(string addr, int port);
  void _handleMessage(string msg);
  void _handleRegistration(string msg);
};

#endif // IRC_SERVER_HPP
