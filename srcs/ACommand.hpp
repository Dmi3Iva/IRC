#ifndef ACOMMAND
#define ACOMMAND

#include "Channel.hpp"
#include "User.hpp"
#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include "responses.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;

class ACommand {
public:
  ACommand(vector<User> *usersPtr, vector<Channel> *channelsPtr);
  ~ACommand(){};
  virtual void execute(User *user, string cmd) = 0;
  void sendMessage(int fd, string msg);

protected:
  string          _name;
  string          _description;
  vector<User>    *_usersPtr;
  vector<Channel> *_channelsPtr;
};

#endif