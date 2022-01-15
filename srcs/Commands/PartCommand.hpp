#ifndef PARTCOMMAND_HPP
#define PARTCOMMAND_HPP

#include "../ACommand.hpp"
#include "../responses.hpp"
#include "../User.hpp"
#include "../utils.hpp"
#include <iostream>
#include <utility>

using std::cout;
using std::endl;
using std::string;
using std::vector;

class ACommand;
class Context;
class User;

class PartCommand : public ACommand {
public:
  PartCommand(string serverName, userVector *usersPtr, channelMap *channelsPtr);
  void execute(User *user, string cmd);
};

#endif
