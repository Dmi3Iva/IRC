//
// Created by Twycross Redfort on 1/24/22.
//

#include "ListCommand.hpp"

ListCommand::ListCommand(string serverName, ACommand::userVector* usersPtr, ACommand::channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr, "LIST")
{
	_description = "[<channel>{,<channel>} [<server>]]";
}

void ListCommand::execute(User* user, string cmd)
{
	cout << "List executes: " << cmd << endl;
	if (!user->isRegistered()) {
		sendMessage(user->getFD(), ERR_NOTREGISTERED(_serverName, user->getNickname(), _name));
		return;
	}
	if (cmd.empty()) {
		for (channelMap::const_iterator channelIt = _channelsPtr->begin(); channelIt != _channelsPtr->end(); ++channelIt)
			_sendListOfChannelTopics(user, channelIt->second);
	}

	vector<string> args = ft_split(cmd, " ");
	if (args.size() == 1) {
		vector<string> channelNames = ft_split(cmd, ",");

	}
	string listOfTopics;
	if (channelNames.size() == 0) {

	} else {
		for (size_t i = 0; i < channelNames.size(); ++i) {
			if (isChannelName(channelNames[i])) {
				channelMap::const_iterator channelIt = _channelsPtr->find(channelNames[i]);
				Channel* channel = channelIt->second;
				if (channelIt != _channelsPtr->end() && !channel->isUserMember(user) && !channel->isPrivate()) {
					listOfTopics = channel->getTopic();
					//					sendMessage(user->getFD(), RPL_NAMREPLY(_serverName, channel->getName(), listOfTopics));
					//					sendMessage(user->getFD(), RPL_ENDOFNAMES(_serverName, channel->getName()));
				}
			}
		}
	}
}

void ListCommand::_sendListOfChannelTopics(User* user, Channel* channel)
{
	if (!channel->isUserMember(user) && !channel->isSecret() && !channel->isPrivate()) {
		for (Channel::usersVectorType::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); ++it) {
			if (!(*it)->isInvisible())
				sendMessage(user->getFD(), RPL_NAMREPLY(_serverName, channel->getNameWithPrefix(), channel->getUserNicknameWithPrefix(*it), user->getNickname()));
		}
		sendMessage(user->getFD(), RPL_ENDOFNAMES(_serverName, channel->getName()));
	}
}

int ListCommand::_getNumberOfVisibleUsers(vector<User*>& users)
{
	int numberOfVisibleUsers = 0;
	for (int i = 0; i < users.size(); ++i) {
		if (!users[i]->isInvisible())
			++numberOfVisibleUsers;
	}
	return numberOfVisibleUsers;
}
