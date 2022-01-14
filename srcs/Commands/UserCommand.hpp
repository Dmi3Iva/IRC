#ifndef USERCOMMAND
#define USERCOMMAND

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
  UserCommand(vector<User> *usersPtr, vector<Channel> *channelsPtr);
  void execute(User *user, string cmd);
};

#endif