#ifndef USER_HPP
#define USER_HPP

#include "Channel.hpp"
#include "constants.hpp"
#include "utils.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h> // For sockaddr_in
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h> // For socket functions
#include <unistd.h>
#include <utility>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::getline;
using std::make_pair;
using std::map;
using std::pair;
using std::string;
using std::vector;

class Context;
class Channel;

class User {
public:
  typedef map<string, Channel *> userChannels;

  // TODO::
  //  User(const User &user) {  }
  // TODO::
  //  User &operator=(const User &user) {  };

  User(int userFd, string hostname, string port);
  ~User();

  void closeFD();
  int getFD();
  void setNickname(string nickname);
  void setUsername(string username);
  void setRealname(string realname);
  string getNickname();
  string getUsername();
  string getRealname();

  bool getIsRegistered();
  const string &getHostname() const;
  const string &getPort() const;
  void setIsRegistered(bool isRegistered);

  void addChannel(Channel *pChannel);
  int quitChannel(string channelName);
  int getMaxOfChannels() const;
  bool isFullOfChannels();

private:
  int _fd;
  string _nickname;
  string _username;
  string _realname;
  string _hostname;
  string _port;
  bool _isRegistered;
  userChannels _channels;
  static const int _MAX_OF_CHANNELS = 10;
};

#endif
