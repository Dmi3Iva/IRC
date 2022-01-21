#include "Context.hpp"

Context::Context() {
  _serverName = "localhost";
  _setupCommands();
}

void Context::_setupCommands() {
  _commandsMap["NICK"] = new NickCommand(_serverName, &_users, &_channels);
  _commandsMap["USER"] = new UserCommand(_serverName, &_users, &_channels);
  _commandsMap["JOIN"] = new JoinCommand(_serverName, &_users, &_channels);
  _commandsMap["PART"] = new PartCommand(_serverName, &_users, &_channels);
  _commandsMap["PING"] = new PingCommand(_serverName, &_users, &_channels);
  _commandsMap["PRIVMSG"] = new PrivateMessageCommand(_serverName, &_users, &_channels);
  _commandsMap["NOTICE"] = new PrivateMessageCommand(_serverName, &_users, &_channels);
}

Context::~Context() {
  fullDeleteMapContainer(_commandsMap);
  fullDeleteMapContainer(_channels);
  fullDeleteContainer(_users);
}

void Context::addUser(int userfd, string hostname, string port) { _users.push_back(new User(userfd, hostname, port)); }

void Context::listenUsers() {
  // TODO:: check all users fds here
  for (vector<User *>::iterator it = _users.begin(), ite = _users.end(); it != ite; ++it) {
    pollfd *userfdArr = getPollFdFromFd((*it)->getFD());
    // TODO:: move poll
    int ret = poll(userfdArr, 1, 800);
    cout << "User poll status " << ret << " revents: " << userfdArr[0].revents << endl;

    // proccess request
    if (ret && (userfdArr[0].revents & POLLIN)) {
      {
        char buffer[IRC_MSG_LEN];
        int fd = (*it)->getFD();
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
          _handleMessage(&(**it), string(buffer));
        } else {
          _users.erase(it);
          cout << "Client ended the _userfd!" << endl;
        }
      }
    }

    delete userfdArr;
  }
}

void Context::clearEmptyData() {
  for (map<string, Channel *>::iterator it = _channels.begin(), ite = _channels.end(); it != ite; ++it)
    if (it->second->getMembers().empty())
      _channels.erase(it);
}

/**
 * Parse and execute user string command
 * @param user
 * @param stringCommand
 * @return
 */
int Context::_executeCommand(User *user, string stringCommand) {
  vector<string> commands;
  size_t pos = stringCommand.find(' ');
  string commandName = stringCommand.substr(0, pos);
  commandsMapType::iterator it = _commandsMap.find(commandName);
  if (it != _commandsMap.end()) {
    cout << "founded command: " << commandName << endl;
    if (pos == string::npos) {
      it->second->execute(user, ""); // maybe it's definitely not enough params?
    } else {
      it->second->execute(user, trim(stringCommand.substr(pos)));
    }
  } else {
    // if User is registered we should reply with error that error wasn't found
    if (user->getIsRegistered()) {
      sendMessage(user->getFD(), ERR_UNKNOWNCOMMAND(_serverName, user->getNickname(), commandName));
    }
  }
  return 0;
}

void Context::_handleMessage(User *user, string msg) {
  // parse
  vector<string> commands = ft_split(msg, DELIMITER);
  // execute in order
  for (vector<string>::iterator it = commands.begin(), ite = commands.end(); it != ite; ++it) {
    _executeCommand(user, trim(*it));
  }
}