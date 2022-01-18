#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

#include "Channel.hpp"
#include "responses.hpp"
#include "User.hpp"
#include <iostream>
#include <map>
#include <string>
#include <sys/socket.h>

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

class ACommand {
public:
  typedef vector<User *> userVector;
  typedef map<string, Channel *> channelMap;

  ACommand(string serverName, userVector *usersPtr, channelMap *channelsPtr);
  ~ACommand(){};
  virtual void execute(User *user, string cmd) = 0;
  void sendMessage(int fd, string msg);
  int isPUserInVector(User *pUser, vector<User *> userList);

protected:
  string _name;
  string _serverName;
  userVector *_usersPtr;
  channelMap *_channelsPtr;
};
#endif