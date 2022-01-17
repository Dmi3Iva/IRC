#ifndef NICKCOMMAND
#define NICKCOMMAND

#include "../ACommand.hpp"
#include "../Context/Context.hpp"
#include "../responses.hpp"
#include <iostream>

class ACommand;

class NickCommand : public ACommand {
public:
  NickCommand(vector<User> *usersPtr, vector<Channel> *channelsPtr);
  void execute(User *user, string cmd);
};

#endif