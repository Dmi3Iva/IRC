#include "InviteCommand.hpp"

InviteCommand::InviteCommand(string serverName, userVector* userPtr, channelMap* channelsPtr)
	: ACommand(serverName, userPtr, channelsPtr, "INVITE")
{
	_description = "INVITE <nickname> <channel>]";
}

InviteCommand::~InviteCommand() { }

void InviteCommand::execute(User* user, string cmd)
{
	if (!user->isRegistered()) {
		user->appendBuffer(ERR_NOTREGISTERED(_serverName, user->getNickname(), "INVITE"));
//		sendMessage(user->getFD(), ERR_NOTREGISTERED(_serverName, user->getNickname(), "INVITE"));
		return;
	}
	vector<string> params = ft_split(cmd, " ");
	if (params.size() != 2) {
		user->appendBuffer(ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), "INVITE"));
//		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), "INVITE"));
		return;
	}
	User* userToInvite = getUserFromArray(params[0]);
	if (!userToInvite) {
		sendMessage(user->getFD(), ERR_NOSUCHNICK(_serverName, user->getNickname(), params[0]));
		return;
	}
	channelMap::iterator channel = _channelsPtr->find(params[1]);
	if (channel == _channelsPtr->end()) {
		sendMessage(user->getFD(), ERR_NOSUCHNICK(_serverName, user->getNickname(), params[1]));
		return;
	}
	_sendInvite(user, channel->second, userToInvite);
}

void InviteCommand::_sendInvite(User* user, Channel* channel, User* userToInvite)
{
	if (!channel->isUserMember(user)) {
		sendMessage(user->getFD(), ERR_NOTONCHANNEL(_serverName, user->getNickname(), channel->getName()));
		return;
	}
	if (channel->isUserMember(userToInvite)) {
		sendMessage(user->getFD(), ERR_USERONCHANNEL(_serverName, user->getNickname(), userToInvite->getNickname(), channel->getName()));
		return;
	}
	if (channel->isInviteOnlyChannel() && !channel->isOperator(user)) {
		sendMessage(user->getFD(), ERR_CHANOPRIVSNEEDED(_serverName, user->getNickname(), channel->getName()));
		return;
	}
	if (channel->isInviteOnlyChannel()) {
		channel->addUserToInviteList(userToInvite);
	}
	sendMessage(user->getFD(), RPL_INVITING(_serverName, user->getNickname(), channel->getName(), userToInvite->getNickname()));
	sendMessage(userToInvite->getFD(), INVITE_RPL(user->getNickname(), user->getUsername(), user->getHostname(), userToInvite->getNickname(), channel->getName()));
	if (userToInvite->getIsAway()) {
		sendMessage(user->getFD(), RPL_AWAY(_serverName, user->getNickname(), userToInvite->getNickname(), userToInvite->getAwayMessage()));
	}
}