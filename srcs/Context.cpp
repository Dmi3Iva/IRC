#include "Context.hpp"

Context::Context()
{
	_serverName = "localhost";
	_setupCommands();
}

void Context::_setupCommands()
{
	_commandsMap["NICK"] = new NickCommand(_serverName, &_users, &_channels);
	_commandsMap["USER"] = new UserCommand(_serverName, &_users, &_channels);
	_commandsMap["JOIN"] = new JoinCommand(_serverName, &_users, &_channels);
	_commandsMap["PART"] = new PartCommand(_serverName, &_users, &_channels);
	_commandsMap["PING"] = new PingCommand(_serverName, &_users, &_channels);
	_commandsMap["PRIVMSG"] = new PrivateMessageCommand(_serverName, &_users, &_channels);
	_commandsMap["NOTICE"] = new PrivateMessageCommand(_serverName, &_users, &_channels);
}

Context::~Context()
{
	fullDeleteMapContainer(_commandsMap);
	fullDeleteMapContainer(_channels);
	fullDeleteContainer(_users);
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

void Context::clearEmptyData()
{
	for (map<string, Channel*>::iterator it = _channels.begin(), ite = _channels.end(); it != ite; ++it)
		if (it->second->getMembers().empty())
			_channels.erase(it);
}

/**
 * Parse and execute user string command
 * @param user
 * @param stringCommand
 * @return
 */
int Context::_executeCommand(User* user, string stringCommand)
{
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

void Context::_handleMessage(User* user)
{
	// parse
	vector<string> commands = ft_split(user->getMessage(), DELIMITER);
	// execute in order
	for (vector<string>::iterator it = commands.begin(), ite = commands.end(); it != ite; ++it) {
		_executeCommand(user, trim(*it));
	}
}