#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP
#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h> // For sockaddr_in
#include <poll.h>
#include <sys/socket.h> // For socket functions
#include <unistd.h>
#include <vector>

#include "User.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Server {
public:
  static Server *getInstance() {
    static Server instance;
    return &instance;
  }

  pollfd *getPollFdFromFd(int fd) {
    pollfd *pollfdArr = new pollfd[1];
    pollfdArr[0].fd = fd;
    pollfdArr[0].events = POLLIN | POLLOUT;
    pollfdArr[0].revents = 0;
    return pollfdArr;
  }

  void start() {
    _init("127.0.0.1", 6667);
    _startListening(_socketfd);

    // Grab a userfd from the queue
    pollfd *serverPollFd = getPollFdFromFd(_socketfd);

    while (!_stopFlag) {
      // accept new users
      _acceptNewClients(serverPollFd);
      _checkUsers();
    }

    delete serverPollFd;
    Users.clear();
    close(_socketfd);
    cout << "The server closed" << endl;
  }

  ~Server() {
    // Close the connection
    close(_socketfd);
  }

private:
  int _socketfd;
  sockaddr_in _sockaddr;
  std::vector<User> Users;
  int _stopFlag;

  void _init(string addr, int port) {
    _sockaddr.sin_family = AF_INET;
    _sockaddr.sin_addr.s_addr = inet_addr(addr.c_str());
    _sockaddr.sin_port = htons(port); // htons is necessary to convert a number to network byte order
    if (bind(_socketfd, reinterpret_cast<const struct sockaddr *>(&_sockaddr), sizeof(_sockaddr)) < 0) {
      cerr << "Failed to bind to port 9999. errno: " << errno << endl;
      exit(EXIT_FAILURE);
    }
  }

  // Start listening. Keep as many connections as possible (SOMAXCONN)
  void _startListening(int socketfd) {
    if (listen(socketfd, SOMAXCONN) < 0) {
      cerr << "Failed to listen on socket. errno: " << errno << endl;
      exit(EXIT_FAILURE);
    }
  }

  Server() {
    _stopFlag = 0;
    _socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if (_socketfd == -1) {
      cerr << "Failed to create socket. errno: " << errno << endl;
      exit(EXIT_FAILURE);
    }
    if (fcntl(_socketfd, F_SETFL, O_NONBLOCK) == -1) {
      cerr << "Failed to update socket fd. errno: " << errno << endl;
      exit(EXIT_FAILURE);
    }
  }

  Server(Server const &);         // Don't Implement.
  void operator=(Server const &); // Don't implement

  void _checkUsers() {
    for (std::vector<User>::iterator it = Users.begin(), ite = Users.end(); it != ite; ++it) {
      pollfd *userfdArr = getPollFdFromFd(it->getFD());
      int ret = poll(userfdArr, 1, 800);
      cout << "User poll status " << ret << " revents: " << userfdArr[0].revents << endl;
      if (ret && (userfdArr[0].revents & POLLIN) && it->listen()) {
        it->closeFD();
        Users.erase(it);
      }
      delete userfdArr;
    }
  }

  void _acceptNewClients(pollfd *serverPollFd) {
    int newConnectionsCount = poll(serverPollFd, 1, 500);
    cout << "New connections count: " << newConnectionsCount << endl;
    if (newConnectionsCount > 0) {
      size_t addrlen = sizeof(sockaddr);
      int userfd =
          accept(_socketfd, reinterpret_cast<struct sockaddr *>(&_sockaddr), reinterpret_cast<socklen_t *>(&addrlen));

      if (userfd < 0) {
        cerr << "Failed to accept new connection. errno: " << errno << endl;
      } else {
        fcntl(userfd, F_SETFL, O_NONBLOCK);
        Users.push_back(User(userfd));
        char host[NI_MAXHOST];    // Client's remote name
        char service[NI_MAXSERV]; // Service (i.e. port) the client is connect on
        memset(host, 0, NI_MAXHOST);
        memset(service, 0, NI_MAXSERV);
        cout << "new user fd is: " << userfd;
        if (getnameinfo(reinterpret_cast<const struct sockaddr *>(&_sockaddr), sizeof(sockaddr), host, NI_MAXHOST,
                        service, NI_MAXSERV, 0) != 0) {
          inet_ntop(AF_INET, &_sockaddr.sin_addr, host, NI_MAXHOST);
        }
        cout << " connected on host:" << host << " port: " << service << endl;
      }
    }
  }
};

#endif // IRC_SERVER_HPP
