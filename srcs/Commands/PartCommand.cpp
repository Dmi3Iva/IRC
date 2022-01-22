#include "PartCommand.hpp"

PartCommand::PartCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr, "PART")
{
	_description = "PART <channel>{,<channel>}";
}

void PartCommand::execute(User* user, string cmd)
{
	if (!user->isRegistered()) {
		sendMessage(
			user->getFD(),
			ERR_NOTREGISTERED(_serverName, (user->getNickname().empty() ? std::string("*") : user->getNickname()), _name));
		return;
	}
	vector<string> channelNames = ft_split(cmd, ",");

	if (channelNames.empty()) {
		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), _name));
		return;
	}
	for (vector<string>::iterator it = channelNames.begin(), ite = channelNames.end(); it != ite; ++it) {
		// user should quit Channel
		channelMap::iterator chIterator = _channelsPtr->find(*it);
		if (chIterator != _channelsPtr->end()) {
			chIterator->second->sendToAllChannelMembers(PART_RPL(user->getNickname(), user->getUsername(),
				user->getHostname(), chIterator->second->getName()));
			user->quitChannel(chIterator->second->getName());
		} else {
			sendMessage(user->getFD(), ERR_NOSUCHCHANNEL(_serverName, user->getNickname(), _name));
		}
	}
}
