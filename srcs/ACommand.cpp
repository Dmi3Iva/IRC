#include "ACommand.hpp"

ACommand::ACommand(string serverName, userVector *usersPtr, channelMap *channelsPtr)
    : _serverName(serverName), _usersPtr(usersPtr), _channelsPtr(channelsPtr) {}

void ACommand::sendMessage(int fd, string msg) {
  cout << "sending message to " << fd << ": " << msg << endl;
  send(fd, msg.c_str(), msg.size(), 0);
}

User	*ACommand::getUserFromArray(string userNick) {
	
	for (userVector::iterator it = _usersPtr->begin(); it != _usersPtr->end(); it++) {
		if ((*it)->getNickname() == userNick) {
			return (*it);
		}
	}
	return (nullptr);
}
