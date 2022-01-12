#ifndef IRC_USER_HPP
#define IRC_USER_HPP

#include "constants.h"
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

using std::cerr;
using std::cout;
using std::endl;

class User {
public:
  // TODO::
  User() {}
  // TODO::
  //  User(const User &user) {  }
  // TODO::
  //  User &operator=(const User &user) {  };

  ~User() {}

  User(int userfd) {
    cout << "User was refreshed" << endl;
    _fd = userfd;
  }

  int listen() {
    // Read from the _userfd
    char buffer[IRC_MSG_LEN];
    ssize_t bytesRead;
    cout << "User listens:" << endl;
    if ((bytesRead = recv(_fd, buffer, sizeof(buffer), 0))) {
      if (bytesRead == -1) {
        cerr << "Something went wrong while try to receive message. errno: " << errno << " fd was: " << _fd << endl;
        return 1;
      }
      buffer[bytesRead] = '\0';
      cout << "The message was: " << buffer;
      parse(string(buffer));
      // Should we parse it?
      // Send a message to the _userfd
      // send(_fd, buffer, bytesRead + 1, 0);
      return 0;
    }
    cout << "Client ended the _userfd!" << endl;
    return 1;
  }

  void closeFD() { close(_fd); }

  int getFD() { return _fd; }

private:
  int _fd;

  void parse(string msg) {
    vector<srting> commands;
    srting stringCommand;
    // split commands
    if (!msg.empty()) {
      while (std::getline(msg, stringCommand, '\n')) {
        commands.push_back(stringCommand);
      }
    }
    // execute them one by one

  }
};

#endif
