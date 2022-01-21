#include "ACommand.hpp"

ACommand::ACommand(string serverName, userVector *usersPtr, channelMap *channelsPtr)
    : _serverName(serverName), _usersPtr(usersPtr), _channelsPtr(channelsPtr) {}

User *ACommand::getUserFromArray(string userNick) {

  for (userVector::iterator it = _usersPtr->begin(); it != _usersPtr->end(); it++) {
    if ((*it)->getNickname() == userNick) {
      return (*it);
    }
  }
  return (NULL);
}

ACommand::~ACommand() {}
