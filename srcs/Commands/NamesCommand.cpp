//
// Created by Twycross Redfort on 1/24/22.
//

#include "NamesCommand.hpp"

NamesCommand::NamesCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr, "NAMES")
{
	_description = "[<channel>{,<channel>}]";
}

void NamesCommand::execute(User* user, string cmd)
{
	cout << "Names executes: " << cmd << endl;
	if (!user->isRegistered()) {
		sendMessage(user->getFD(), ERR_NOTREGISTERED(_serverName, user->getNickname(), _name));
		return;
	}
	vector<string> channelNames = ft_split(cmd, ",");
	string listOfNicknames;
	if (channelNames.size() == 0) {
		for (channelMap::const_iterator channelIt = _channelsPtr->begin(); channelIt != _channelsPtr->end(); ++channelIt) {
			Channel* channel = channelIt->second;
			if (!channel->isUserMember(user) && !channel->isOnlyInviteChannel()) {
				listOfNicknames = getListOfNicknames(channel->getMembers(), " ");
				sendMessage(user->getFD(), RPL_NAMREPLY(_serverName, channel->getName(), listOfNicknames));
				sendMessage(user->getFD(), RPL_ENDOFNAMES(_serverName, channel->getName()));
			}
		}
	} else {
		for (size_t i = 0; i < channelNames.size(); ++i) {
			if (isChannelName(channelNames[i])) {
				channelMap::const_iterator channelIt = _channelsPtr->find(channelNames[i]);
				Channel* channel = channelIt->second;
				if (channelIt != _channelsPtr->end() && !channel->isUserMember(user) && !channel->isOnlyInviteChannel()) {
					listOfNicknames = getListOfNicknames(channel->getMembers(), " ");
					sendMessage(user->getFD(), RPL_NAMREPLY(_serverName, channel->getName(), listOfNicknames));
					sendMessage(user->getFD(), RPL_ENDOFNAMES(_serverName, channel->getName()));
				}
			}
		}
	}
}
