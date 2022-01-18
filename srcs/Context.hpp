#ifndef CONTEXT
#define CONTEXT

#include "ACommand.hpp"
#include "Channel.hpp"
#include "Commands/NickCommand.hpp"
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
  vector<User> _users;
  vector<Channel> _channels;

  User* findUserByFd(int);
  void _setupCommands();
  int _executeCommand(User *user, string stringCommand);

public:
  Context();
  ~Context();

  void addUser(int userfd);
  void deleteUser(int userfd);
  void _handleMessage(int userfd, string msg);
};

#endif