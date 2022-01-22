#include "PingCommand.hpp"

PingCommand::PingCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr, "PING")
{
	_description = "PING <server1> [<server2>]";
}

void PingCommand::execute(User* user, string cmd)
{
	if (!user->isRegistered()) {
		sendMessage(
			user->getFD(),
			ERR_NOTREGISTERED(_serverName, (user->getNickname().empty() ? std::string("*") : user->getNickname()), _name));
		return;
	}
	if (cmd.empty()) {
		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), _name));
		return;
	}
	if (cmd[0] == ':') {
		cmd.erase(0, 1);
		// this error appears only in the case of message without white spaces ":"
		if (cmd.empty()) {
			sendMessage(user->getFD(), ERR_NOORIGIN(_serverName, user->getNickname()));
			return;
		}
	}
	sendMessage(user->getFD(), PONG_RPL(cmd));
}
