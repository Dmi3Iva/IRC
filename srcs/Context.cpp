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

void Context::addUser(User* user) { _users.push_back(user); }

void Context::deleteUser(User* user)
{
	for (std::vector<User*>::iterator it = _users.begin(), ite = _users.end(); it != ite; ++it)
		if ((*it)->getFD() == user->getFD())
			_users.erase(it);
}

User* Context::findUserByFd(int fd)
{
	for (size_t i = 0; i < _users.size(); ++i) {
		if (_users[i]->getFD() == fd)
			return _users[i];
	}
	return NULL;
}

void Context::clearEmptyData()
{
	map<string, Channel*>::iterator it = _channels.begin(), ite = _channels.end();

	while (it != ite) {
		if (it->second->getMembers().empty()) {
			delete it->second;
			it = _channels.erase(it);
		} else {
			++it;
		}
	}
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
		if (user->isRegistered()) {
			sendMessage(user->getFD(), ERR_UNKNOWNCOMMAND(_serverName, user->getNickname(), commandName));
		}
	}
	return 0;
}

void Context::handleMessage(User* user, string msg)
{
	cout << "Handle message: " << msg << endl;
	string newMessage = _parseMessage(user, msg);
	if (!newMessage.empty()) {
		// parse
		vector<string> commands = ft_split(msg, "\n");
		// execute in order
		for (vector<string>::iterator it = commands.begin(), ite = commands.end(); it != ite; ++it) {
			_executeCommand(user, trim(*it));
		}
	}
}

string Context::_parseMessage(User* user, string buffer)
{
	// remove all \r from the string
	buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.end());
	// retrieve previous message
	string wholeMessage = user->getMessage() + buffer;
	// find for delimiter
	size_t pos = wholeMessage.rfind('\n');
	if (pos == string::npos) {
		cout << "It is not whole message just keep it " << wholeMessage;
		user->setMessage(wholeMessage);
		return "";
	}
	string newMessage = wholeMessage.substr(0, pos);
	user->setMessage(wholeMessage.substr(pos + 1));
	return newMessage;
}