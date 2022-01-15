//
// Created by Shandy Mephesto on 1/15/22.
//

#include "PartCommand.hpp"
PartCommand::PartCommand(string serverName, userVector *usersPtr, channelMap *channelsPtr)
    : ACommand(serverName, usersPtr, channelsPtr) {
  _name = "PART";
}

void PartCommand::execute(User *user, string cmd) {
  vector<string> channelNames = ft_split(cmd, ",");
  if (channelNames.empty()) {
    sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), _name));
    return;
  }
  for (vector<string>::iterator it = channelNames.begin(), ite = channelNames.end(); it != ite; ++it) {
    // user should quit Channel
    channelMap::iterator chIterator = _channelsPtr->find(*it);
    if (chIterator != _channelsPtr->end()) {
      for (Channel::usersVectorType::const_iterator it = chIterator->second.getMembers().begin(),
                                                    ite = chIterator->second.getMembers().end();
           it != ite; ++it)
        sendMessage((*it)->getFD(), PART_REPL(user->getNickname(), user->getUsername(), user->getHostname(),
                                              chIterator->second.getName()));
      user->quitChannel(chIterator->second.getName());
      return;
    } else {
      sendMessage(user->getFD(), ERR_NOSUCHCHANNEL(_serverName, user->getNickname(), _name));
      return;
    }
  }

  // resend user message to channel members
}
