#ifndef NICKCOMMAND
#define NICKCOMMAND

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
  NickCommand(vector<User> *usersPtr, vector<Channel> *channelsPtr);
  void execute(User *user, string cmd);
};

#endif