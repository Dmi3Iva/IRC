#ifndef USERCOMMAND_HPP
#define USERCOMMAND_HPP

#include "../ACommand.hpp"
#include "../Context.hpp"
#include "../utils.hpp"
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

class ACommand;
class Context;
class User;

class UserCommand : public ACommand {
public:
  UserCommand(string serverName, vector<User> *usersPtr, channelMap *channelsPtr);
  void execute(User *user, string cmd);
};

#endif