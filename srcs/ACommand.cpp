#include "ACommand.hpp"

ACommand::ACommand(vector<User> *usersPtr, vector<Channel> *channelsPtr)
    : _usersPtr(usersPtr), _channelsPtr(channelsPtr) {}

void ACommand::sendMessage(int fd, string msg) { send(fd, msg.c_str(), msg.size(), 0); }

User	*ACommand::getUserFromArray(string userNick) {
	
	for (vector<User>::iterator it = _usersPtr->begin(); it != _usersPtr->end(); it++) {
		if (it->getNickname() == userNick) {
			return (&(*it));
		}
	}
	return (nullptr);
}
