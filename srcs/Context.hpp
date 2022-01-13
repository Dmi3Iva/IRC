#ifndef CONTEXT
#define CONTEXT

#include "ACommand.hpp"
#include "Commands/NickCommand.hpp"
#include "Commands/UserCommand.hpp"
#include "Server.hpp"
#include <map>
#include <utility>

using std::cout;
using std::endl;
using std::make_pair;
using std::map;
using std::string;

class Server;
class ACommand;

class Context {
private:
  typedef map<string, ACommand *> commandsMapType;
  commandsMapType _commandsMap;
  Server *_serverPtr;

public:
  Context();
  Context(Server *serverPtr);
  ~Context();

  void setupCommands();
  void setServer(Server *serverPtr);
  int executeCommand(User *user, string stringCommand);
  void sendMessage(int whereFd, string responseMessage);
};

#endif