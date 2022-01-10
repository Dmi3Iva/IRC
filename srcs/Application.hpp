//
// Created by Shandy Mephesto on 1/10/22.
//

#ifndef IRC_APPLICATION_HPP
#define IRC_APPLICATION_HPP
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

class Application {
public:
  static Application *getInstance() {
    static Application instance;
    return &instance;
  }

  void start() {
    // TODO:: there is suggestion to use poll
    // Start listening. Keep as many connections as possible (SOMAXCONN)
    if (listen(_socketfd, SOMAXCONN) < 0) {
      cerr << "Failed to listen on socket. errno: " << errno << endl;
      exit(EXIT_FAILURE);
    }
    cout << "listen was successful" << endl;

    // Grab a userfd from the queue
    pollfd mypoll;
    mypoll.fd = _socketfd;
    mypoll.events = POLLIN | POLLOUT;
    mypoll.revents = POLLIN | POLLOUT;
    pollfd pollfdArr[1] = {mypoll};
    while (!_stopFlag) {
      // accept new users
      int ret = poll(pollfdArr, 1, 500);
      cout << "pol return: " << ret << endl;
      if (ret) {
        size_t addrlen = sizeof(sockaddr);
        int userfd =
            accept(_socketfd, reinterpret_cast<struct sockaddr *>(&_sockaddr), reinterpret_cast<socklen_t *>(&addrlen));

        if (userfd < 0) {
          cerr << "Failed to grab userfd. errno: " << errno << endl;
          //          exit(EXIT_FAILURE);
        } else {

          //          int flags = fcntl(userfd, F_GETFL);
          //          flags &= ~O_NONBLOCK;
          //          fcntl(userfd, F_SETFL, flags);
          fcntl(userfd, F_SETFL, O_NONBLOCK);
          Users.push_back(User(userfd));
          char host[NI_MAXHOST];       // Client's remote name
          char service[NI_MAXSERV];    // Service (i.e. port) the client is connect on
          memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
          memset(service, 0, NI_MAXSERV);
          cout << "new user fd is: " << userfd << std::endl;
          if (getnameinfo(reinterpret_cast<const struct sockaddr *>(&_sockaddr), sizeof(sockaddr), host, NI_MAXHOST,
                          service, NI_MAXSERV, 0) == 0) {
            cout << host << " connected on port " << service << endl;
          } else {
            inet_ntop(AF_INET, &_sockaddr.sin_addr, host, NI_MAXHOST);
            cout << host << " connected on port " << ntohs(_sockaddr.sin_port) << endl;
          }
        }
      }

      for (std::vector<User>::iterator it = Users.begin(), ite = Users.end(); it != ite; ++it) {
        pollfd userPoll;
        userPoll.fd = it->getFD();
        userPoll.events = POLLIN | POLLOUT;
        userPoll.revents = POLLIN | POLLOUT;
        pollfd userfdArr[1] = {userPoll};
        int ret = poll(userfdArr, 1, 500);
        cout << "User status " << ret << endl;
        if (ret && it->listen()) {
          it->closeFD();
          Users.erase(it);
        }
      }
    }

    Users.clear();
    close(_socketfd);
    cout << "The server closed" << endl;
  }

  void stop() { _stopFlag = 1; }

  ~Application() {
    // Close the connections
    close(_socketfd);
  }

private:
  int _socketfd;
  sockaddr_in _sockaddr;
  std::vector<User> Users;
  int _stopFlag;

  Application() {
    _stopFlag = 0;
    _socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketfd == -1) {
      cerr << "Failed to create socket. errno: " << errno << endl;
      exit(EXIT_FAILURE);
    }
    cout << "Socket created" << endl;

    int flags = fcntl(_socketfd, F_GETFL);
    flags &= ~O_NONBLOCK;
    fcntl(_socketfd, F_SETFL, flags);
    cout << "Socket fd was updated" << endl;
    // Listen to port 6667 on any address
    _sockaddr.sin_family = AF_INET;
    _sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    _sockaddr.sin_port = htons(6667);                   // htons is necessary to convert a number to network byte order
    if (bind(_socketfd, reinterpret_cast<const struct sockaddr *>(&_sockaddr), sizeof(_sockaddr)) < 0) {
      cerr << "Failed to bind to port 9999. errno: " << errno << endl;
      exit(EXIT_FAILURE);
    }
    cout << "Bound server socket" << endl;
  }
  Application(Application const &);    // Don't Implement.
  void operator=(Application const &); // Don't implement
};

#endif // IRC_APPLICATION_HPP
