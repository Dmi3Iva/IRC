#infdef CONTEXT
#define CONTEXT

#include "Commands/NickCommand.hpp"
#include "Commands/QuitCommand.hpp"
#include "Commands/UserCommand.hpp"
#include <map>

class Context {
public:
  Context(Server *serverPtr) : _serverPtr(serverPtr) {
    commandsMap = {{"NICK", NickCommand}, {"USER", UserCommand}, {"QUIT", QuitCommand}};
  }
  ~Context() {}

  executeCommand(string cmd, string restString) {
    for (commandsMapType::iterator it = commandsMap.begin(), ite = commandsMap.end(); it != ite; ++it) {
      if (it->first == cmd) {
        cout << "founded command: " << cmd << endl;
        it->second.execute(restString);
        break;
      }
    }

  private:
    typedef map<string, ACommand> commandsMapType;
    commandsMapType commandsMap;
    Server *_serverPtr;
  };

#endif