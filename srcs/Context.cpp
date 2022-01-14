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

void Context::addUser(int userfd) { _users.push_back(User(userfd)); }

void Context::listenUsers() {
  // TODO:: check all users fds here
  for (std::vector<User>::iterator it = _users.begin(), ite = _users.end(); it != ite; ++it) {
    pollfd *userfdArr = getPollFdFromFd(it->getFD());
    // TODO:: move poll
    int ret = poll(userfdArr, 1, 800);
    cout << "User poll status " << ret << " revents: " << userfdArr[0].revents << endl;

    // proccess request
    if (ret && (userfdArr[0].revents & POLLIN)) {
      {
        char buffer[IRC_MSG_LEN];
        int fd = it->getFD();
        ssize_t bytesRead;
        cout << "User listens:" << endl;
        if ((bytesRead = recv(fd, buffer, sizeof(buffer), 0))) {
          if (bytesRead == -1) {
            cerr << "Something went wrong while try to receive message. errno: " << errno << " fd was: " << fd << endl;
          }

          buffer[bytesRead] = '\0';
          if (bytesRead - 1 >= 0 && buffer[bytesRead - 1] == '\r')
            buffer[bytesRead - 1] = '\0';

          cout << "The message was: " << buffer;
          _handleMessage(&(*it), string(buffer));
        } else {
          it->closeFD();
          _users.erase(it);
          cout << "Client ended the _userfd!" << endl;
        }
      }
    }

    delete userfdArr;
  }
}

int Context::_executeCommand(User *user, string stringCommand) {
  vector<string> commands;
  string commandName = stringCommand.substr(0, stringCommand.find(' '));
  for (commandsMapType::iterator it = _commandsMap.begin(), ite = _commandsMap.end(); it != ite; ++it) {
    if (it->first == commandName) {
      cout << "founded command: " << commandName << endl;
      string s = stringCommand.substr(stringCommand.find(' '));
      string executeString = trim(s);
      it->second->execute(user, executeString);
      break;
    }
  }
  return 0;
}

void Context::_handleMessage(User *user, string msg) {
  // parse
  vector<string> commands = ft_split(msg, DELIMITER);
  // execute in order
  for (vector<string>::iterator it = commands.begin(), ite = commands.end(); it != ite; ++it) {
    _executeCommand(user, *it);
  }
}