#include "ACommand.hpp"

ACommand::ACommand(string serverName, userVector *usersPtr, channelMap *channelsPtr)
    : _serverName(serverName), _usersPtr(usersPtr), _channelsPtr(channelsPtr) {}

void ACommand::sendMessage(int fd, string msg) {
  cout << "sending message to " << fd << ": " << msg << endl;
  send(fd, msg.c_str(), msg.size(), 0);
}

int ACommand::isPUserInVector(User *pUser, vector<User *> userList) {
  for (vector<User *>::iterator it = userList.begin(), ite = userList.end(); it != ite; ++it) {
    if ((*it)->getNickname() == pUser->getNickname()) {
      return 1;
    }
  }
  return 0;
}
