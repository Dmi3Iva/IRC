#include "NickCommand.hpp"

NickCommand::NickCommand(vector<User> *usersPtr, vector<Channel> *channelsPtr) : ACommand(usersPtr, channelsPtr) {
  _name = "NICK";
  _description = "NICK <nickname> [ <hopcount> ]";
}

void NickCommand::execute(User *user, string cmd) {
  cout << "Execute command" << cmd << endl;
  user->setNickname(cmd);
  if (!user->getNickname().empty() && !user->getRealname().empty() && !user->getUsername().empty()) {
    user->setIsRegistered(true);
    string msg =
        RPL_MOTDSTART(string("IRCat"), user->getNickname())
        RPL_MOTD(string("IRCat"), user->getNickname())
        RPL_ENDOFMOTD(string("IRCat"), user->getNickname());
    sendMessage(user->getFD(), msg);
  }
}
