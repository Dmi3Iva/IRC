#ifndef IRC_USER_HPP
#define IRC_USER_HPP

#include "constants.hpp"
#include "utils.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h> // For sockaddr_in
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h> // For socket functions
#include <unistd.h>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::vector;

class Context;

class User {
public:
  // TODO::
  //  User(const User &user) {  }
  // TODO::
  //  User &operator=(const User &user) {  };

  User(int userFd);
  ~User();

  //  int listen();
  void closeFD();
  int getFD();
  void setNickname(string nickname);
  void setUsername(string username);
  void setRealname(string realname);
  string getNickname();
  string getUsername();
  string getRealname();

  bool getIsRegistered();
  void setIsRegistered(bool isRegistered);

private:
  int _fd;
  string _nickname;
  string _username;
  string _realname;
  bool _isRegistered;

  //  void _handleMessage(string msg);
  //  void _handleRegistration(string msg);
};

#endif
