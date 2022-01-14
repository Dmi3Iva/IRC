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

// class Server;
// class ACommand;

class Context {
private:
  typedef map<string, ACommand *> commandsMapType;
  commandsMapType _commandsMap;
  std::vector<User> _users;
  std::vector<Channel> _channels;

  void _setupCommands();
  void _handleMessage(User *user, string msg);
  int _executeCommand(User *user, string stringCommand);

public:
  Context();
  ~Context();

  void addUser(int userfd);
  void listenUsers();
  void executeMessage(User u, string msg);
  //  void removeUser(user user);

  // TODO:: move sendMessage to ACommand
  // void sendMessage(int whereFd, string responseMessage);
};

#endif