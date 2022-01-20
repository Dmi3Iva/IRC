#include "Context.hpp"

Context::Context() { _setupCommands(); }

void Context::_setupCommands() {
  // TODO:: replace with init list
  _commandsMap["NICK"] = new NickCommand(&_users, &_channels);
  _commandsMap["USER"] = new UserCommand(&_users, &_channels);
}

Context::~Context() {
  _commandsMap.clear();
  _users.clear();
  _channels.clear();
}

void Context::addUser(User* user) { _users.push_back(*user); }

void Context::deleteUser(User* user) {
  for (std::vector<User>::iterator it = _users.begin(), ite = _users.end(); it != ite; ++it)
    if (it->getFD() == user->getFD())
      _users.erase(it);
}

User* Context::findUserByFd(int fd) {
  for (size_t i = _users.size(); i >= 0; --i) {
   if (_users[i].getFD() == fd)
     return &_users[i];
  }
  return NULL;
}

int Context::_executeCommand(User *user, string stringCommand) {
  vector<string> commands;
  string commandName = stringCommand.substr(0, stringCommand.find(' '));
  commandsMapType::iterator it = _commandsMap.find(commandName);
  if (it != _commandsMap.end()) {
    cout << "founded command: " << commandName << endl;
    string s = stringCommand.substr(stringCommand.find(' '));
    string executeString = trim(s);
    it->second->execute(user, executeString);
  }
  return 0;
}

void Context::_handleMessage(User* user) {
  // parse
  vector<string> commands = ft_split(user->getMessage(), DELIMITER);
  // execute in order
  for (vector<string>::iterator it = commands.begin(), ite = commands.end(); it != ite; ++it) {
    _executeCommand(user, *it);
  }
}