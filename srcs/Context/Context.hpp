#ifndef CONTEXT
#define CONTEXT

#include "../ACommand.hpp"
#include "CommandsHeaders.hpp"
#include "../User.hpp"
#include "../utils.hpp"
#include <map>
#include <utility>

using std::cout;
using std::endl;
using std::make_pair;
using std::map;
using std::string;
using std::vector;

// class Server;
// class ACommand;

class Context {
private:
  typedef map<string, ACommand *> commandsMapType;
  commandsMapType _commandsMap;
  vector<User> _users;
  vector<Channel> _channels;

  void _setupCommands();
  void _handleMessage(User *user, string msg);
  int _executeCommand(User *user, string stringCommand);

public:
  Context();
  ~Context();

  void addUser(int userfd);
  void listenUsers();

  
};

#endif