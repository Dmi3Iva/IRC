#include "NickCommand.hpp"

NickCommand::NickCommand(vector<User> *usersPtr, vector<Channel> *channelsPtr) : ACommand(usersPtr, channelsPtr) {
  _name = "NICK";
}

void NickCommand::execute(User *user, string cmd) {
  cout << "Execute command" << cmd << endl;
  user->setNickname(cmd);
  if (!user->getNickname().empty() && !user->getRealname().empty() && !user->getUsername().empty()) {
    user->setIsRegistered(true);
    sendMessage(user->getFD(), RPL_WELCOME(user->getNickname(), user->getUsername, "localhost"));
  }
};
