#include "KickCommand.hpp"

KickCommand::KickCommand(string serverName, userVector* userPtr, channelMap* channelsPtr)
	: ACommand(serverName, userPtr, channelsPtr, "KICK")
{
	_description = "KICK <channel>{,<channel>} <user>{,<user>} [:<comment>]";
}

KickCommand::~KickCommand() { }

void KickCommand::execute(User* user, string cmd)
{
	if (!user->isRegistered()) {
		sendMessage(user->getFD(), ERR_NOTREGISTERED(_serverName, user->getNickname(), "KICK"));
		return;
	}
	string message = _constructMessageAndEraseFromCmd(cmd);
	vector<string> params = ft_split(cmd, " ");
	if (params.size() != 2) {
		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), "KICK"));
		return;
	}
	vector<string> channels = ft_split(params[0], ",");
	vector<string> users = ft_split(params[1], ",");
	_startKicking(user, channels, users);
}

string KickCommand::_constructMessageAndEraseFromCmd(string& cmd)
{
	bool isStartMessage = false;
	size_t prevPos = 0;
	size_t semicoloms;
	while (!isStartMessage) {
		semicoloms = cmd.find(':', prevPos + 1);
		if (semicoloms == string::npos) {
			return ("");
		} else if (semicoloms != 0 && cmd[semicoloms - 1] == ' ') {
			isStartMessage = true;
		} else {
			prevPos = semicoloms;
		}
	}
	string msg = cmd.substr(semicoloms, cmd.size() - 1);
	cmd.erase(semicoloms, cmd.size());
	msg.erase(remove(msg.begin(), msg.end(), ':'), msg.end());
	return (msg);
}

void KickCommand::_startKicking(User* user, vector<string>& channels, vector<string>& users)
{
	for (vector<string>::iterator chit = channels.begin(); chit != channels.end(); chit++) {
		channelMap::iterator channel = _channelsPtr->find(*chit);
		if (channel != _channelsPtr->end()) {
			if (!channel->second->isUserMember(user)) {
				sendMessage(user->getFD(), ERR_NOTONCHANNEL(_serverName, user->getNickname(), channel->second->getName()));
			}
			else if (!channel->second->isOperator(user)) {
				sendMessage(user->getFD(), ERR_CHANOPRIVSNEEDED(_serverName, user->getNickname(), channel->second->getName()));
			} else {
				_kickUsersFromChannel(user, channel->second, users);
			}
		} else {
			sendMessage(user->getFD(), ERR_NOSUCHCHANNEL(_serverName, user->getNickname(), *chit));
		}
	}
}

void KickCommand::_kickUsersFromChannel(User* user, Channel* channel, vector<string>& users)
{
	for (vector<string>::iterator usit = users.begin(); usit != users.end(); usit++) {
		User *userToKick = channel->getUserFromMembers(*usit);
		if (!userToKick) {
			sendMessage(user->getFD(), ERR_NOTONCHANNEL(_serverName, user->getNickname(), channel->getName()));
		} else {
			channel->removeUser(userToKick);
		}
	}
}