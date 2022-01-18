#ifndef JOINCOMMAND_HPP
#define JOINCOMMAND_HPP

#include "../ACommand.hpp"
#include "../responses.hpp"
#include "../User.hpp"
#include "../utils.hpp"
#include <iostream>
#include <utility>

using std::cout;
using std::endl;
using std::make_pair;
using std::string;
using std::vector;

class ACommand;
class Context;
class User;

class JoinCommand : public ACommand {
public:
  JoinCommand(string serverName, userVector *usersPtr, channelMap *channelsPtr);
  void execute(User *user, string cmd);

private:
  void _joinChannel(User *user, string channelName, string key);
  void _userHasJoinedChannel(User *user, channelMap::iterator chIterator);
};

#endif
