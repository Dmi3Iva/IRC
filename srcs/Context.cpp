#include "Context.hpp"

Context::Context() {}

Context::Context(Server *serverPtr) : _serverPtr(serverPtr) { setupCommands(); }

void Context::setupCommands() {
  _commandsMap["NICK"] = new NickCommand(this);
  _commandsMap["USER"] = new UserCommand(this);
}

void Context::setServer(Server *serverPtr) { _serverPtr = serverPtr; }

Context::~Context() { _commandsMap.clear(); }

int Context::executeCommand(User *user, string stringCommand) {
  string commandName = stringCommand.substr(0, stringCommand.find(' '));
  for (commandsMapType::iterator it = _commandsMap.begin(), ite = _commandsMap.end(); it != ite; ++it) {
    if (it->first == commandName) {
      cout << "founded command: " << commandName << endl;
      it->second->execute(user, stringCommand.substr(stringCommand.find(' ')));
      break;
    }
  }
  return 0;
}

void Context::sendMessage(int whereFd, string responseMessage) {
  send(whereFd, responseMessage.c_str(), responseMessage.size(), 0);
}
