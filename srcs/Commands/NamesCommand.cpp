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
	if (cmd.empty()) {
		for (channelMap::const_iterator channelIt = _channelsPtr->begin(); channelIt != _channelsPtr->end(); ++channelIt)
			_sendListOfChannelMembers(user, channelIt->second);
	} else {
		vector<string> channelNames = ft_split(cmd, ",");
		for (size_t i = 0; i < channelNames.size(); ++i) {
			channelMap::const_iterator channelIt;
			if ((channelIt = _channelsPtr->find(channelNames[i])) != _channelsPtr->end())
				_sendListOfChannelMembers(user, channelIt->second);
		}
	}
}

void NamesCommand::_sendListOfChannelMembers(User* user, Channel* channel)
{
	if (!channel->isSecret() && !channel->isPrivate() && !channel->isUserMember(user)) {
		string listOfNicknames = getListOfNicknames(channel->getMembers(), " ");
		sendMessage(user->getFD(), RPL_NAMREPLY(_serverName, channel->getName(), listOfNicknames, user->getNickname()));
		sendMessage(user->getFD(), RPL_ENDOFNAMES(_serverName, channel->getName()));
	}
}
