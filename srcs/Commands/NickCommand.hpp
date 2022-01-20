#ifndef NICKCOMMAND_HPP
#define NICKCOMMAND_HPP

#include "../ACommand.hpp"
#include "../responses.hpp"
#include <iostream>

class ACommand;

using std::cout;
using std::endl;
using std::string;
using std::vector;

class NickCommand : public ACommand {
public:
  NickCommand(string serverName, vector<User *> *usersPtr, channelMap *channelsPtr);
  void execute(User *user, string cmd);
};

#endif