#include "ACommand.hpp"

ACommand::ACommand(string serverName, userVector *usersPtr, channelMap *channelsPtr)
    : _serverName(serverName), _usersPtr(usersPtr), _channelsPtr(channelsPtr) {}

void ACommand::sendMessage(int fd, string msg) {
  cout << "sending message to " << fd << ": " << msg << endl;
  send(fd, msg.c_str(), msg.size(), 0);
}

void ACommand::sendToAllChannelMembers(Channel *channel, string message) {
  for (Channel::usersVectorType::const_iterator it = channel->getMembers().begin(), ite = channel->getMembers().end();
       it != ite; ++it) {
    sendMessage((*it)->getFD(), message);
  }
}
