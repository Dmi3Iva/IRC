#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

#include "Channel.hpp"
#include "responses.hpp"
#include "User.hpp"
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <sys/socket.h>
#include "responses.hpp"

using std::list;
using std::cout;
using std::endl;
using std::make_pair;
using std::map;
using std::string;
using std::vector;

class ACommand {
public:
  typedef vector<User *> userVector;
  typedef map<string, Channel *> channelMap;

  ACommand(string serverName, userVector *usersPtr, channelMap *channelsPtr);
  virtual ~ACommand();
  virtual void execute(User *user, string cmd) = 0;
  User	*getUserFromArray(string userName);

protected:
  string _name;
  string _serverName;
  string _description;
  userVector *_usersPtr;
  channelMap *_channelsPtr;
};

#endif