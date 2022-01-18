#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "ACommand.hpp"
#include "Channel.hpp"
#include "Commands/JoinCommand.hpp"
#include "Commands/NickCommand.hpp"
#include "Commands/PartCommand.hpp"
#include "Commands/UserCommand.hpp"
#include "User.hpp"
#include "utils.hpp"
#include <map>
#include <utility>

using std::cout;
using std::endl;
using std::make_pair;
using std::map;
using std::string;
using std::vector;

class Context {
private:
  typedef map<string, ACommand *> commandsMapType;
  commandsMapType _commandsMap;
  vector<User *> _users;
  map<string, Channel *> _channels;

  void _setupCommands();
  void _handleMessage(User *user, string msg);
  int _executeCommand(User *user, string stringCommand);

public:
  Context();
  ~Context();

  void addUser(int userfd, string hostname, string port);
  void listenUsers();

  /**
   * Remove empty channels and removed users from storage
   */
  void clearEmptyData();
};

#endif