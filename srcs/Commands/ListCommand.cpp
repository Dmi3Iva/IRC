//
// Created by Twycross Redfort on 1/24/22.
//

#include "ListCommand.hpp"

ListCommand::ListCommand(string serverName, ACommand::userVector* usersPtr, ACommand::channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr, "LIST")
{
	_description = "[<channel>{,<channel>}]";
}

void ListCommand::execute(User* user, string cmd)
{
	cout << "List executes: " << cmd << endl;
	if (!user->isRegistered())
		sendMessage(user->getFD(), ERR_NOTREGISTERED(_serverName, user->getNickname(), _name));
	else if (cmd.empty()) {
		for (channelMap::const_iterator channelIt = _channelsPtr->begin(); channelIt != _channelsPtr->end(); ++channelIt)
			_sendListOfChannelTopics(user, channelIt->second);
		return;
	} else {
		vector<string> channelNames = ft_split(cmd, ",");
		for (size_t i = 0; i < channelNames.size(); ++i) {
			channelMap::const_iterator channelIt;
			if ((channelIt = _channelsPtr->find(channelNames[i])) != _channelsPtr->end())
				_sendListOfChannelTopics(user, channelIt->second);
		}
	}
}

void ListCommand::_sendListOfChannelTopics(User* user, Channel* channel)
{
	if ((!channel->isSecret() && !channel->isPrivate()) || channel->isUserMember(user)) {
		int visible = _getNumberOfVisibleUsers(channel->getMembers());
		string topic = channel->getTopic();
		sendMessage(user->getFD(), RPL_LIST(_serverName, user->getNickname(), channel->getName(), std::to_string(visible), topic));
		sendMessage(user->getFD(), RPL_LISTEND(_serverName, user->getNickname()));
	}
}

int ListCommand::_getNumberOfVisibleUsers(const vector<User*>& users)
{
	int numberOfVisibleUsers = 0;
	for (size_t i = 0; i < users.size(); ++i) {
		if (!users[i]->isInvisible())
			++numberOfVisibleUsers;
	}
	return numberOfVisibleUsers;
}
