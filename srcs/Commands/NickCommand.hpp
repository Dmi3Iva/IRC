#ifndef NICKCOMMAND
#define NICKCOMMAND

#include "../ACommand.hpp"
#include <iostream>

class ACommand;

using std::cout;
using std::endl;
using std::string;

class NickCommand : public ACommand {
public:
  NickCommand(Context *ctx);
  void execute(User *user, string cmd);
};

#endif