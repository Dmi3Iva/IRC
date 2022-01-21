#ifndef SERVER_HPP
#define SERVER_HPP

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
  struct sockaddr_in _address;
  Context* _context;
  string _password;
  pollfdType _pollfds;

public:
  Server(string, string, string);
  ~Server();

  void createSocket();
  void start();
  void polling();
  void acceptNewClients();
  bool receiveMessage(User* user);

private:
  Server(Server const &);         // Don't Implement.
  void operator=(Server const &); // Don't implement.

<<<<<<< HEAD
  pair<string, string> _getConnectionInfo(int userFd);

=======
  void _printConnectionInfo(int userFd);
>>>>>>> 47d11f854bbd249013107e56f971bae3c3de3713
};

#endif
