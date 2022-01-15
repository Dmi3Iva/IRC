//
// Created by Shandy Mephesto on 1/15/22.
//

#ifndef PRIVMSGCOMMAND_HPP
#define PRIVMSGCOMMAND_HPP

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

class PrivmsgCommand : public ACommand {
public:
  PrivmsgCommand(vector<User> *usersPtr, vector<Channel> *channelsPtr);
  void execute(User *user, string cmd);
};

#endif
