#include "TopicCommand.hpp"

TopicCommand::TopicCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr, "TOPIC")
{
	_description = "<channel> [<topic>]";
}

void TopicCommand::execute(User* user, string cmd)
{
	if (cmd.empty())
		return sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), _name));
	size_t pos = cmd.find(" ");
	string channelName = cmd.substr(0, pos);
	string topic = pos == string::npos ? "" : cmd.substr(pos + 1);
	channelMap::iterator chIterator = _channelsPtr->find(channelName);
	if (chIterator == _channelsPtr->end() || (chIterator->second->isSecret() && !chIterator->second->isUserMember(user)))
		return sendMessage(user->getFD(), ERR_NOSUCHCHANNEL(_serverName, user->getNickname(), channelName));
	Channel* ch = chIterator->second;
	if (ch->isPrivate() && !ch->isUserMember(user))
		return sendMessage(user->getFD(), ERR_NOTONCHANNEL(_serverName, user->getNickname(), channelName));
	if (topic.empty()) {
		if (ch->getTopic().empty())
			return sendMessage(user->getFD(), RPL_NOTOPIC(_serverName, channelName));
		else
			return sendMessage(user->getFD(), RPL_TOPIC(_serverName, channelName, ch->getTopic()));
	} else {
		if (ch->isTopicSettableOnlyByOpers() && !ch->isOperator(user))
			return sendMessage(user->getFD(), ERR_CHANOPRIVSNEEDED(_serverName, user->getNickname(), channelName));
		ch->setTopic(topic);
		ch->sendToAllChannelMembers(TOPIC_RPL(user->getNickname(), user->getUsername(), user->getHostname(), channelName, topic));
	}
}