//
// Created by Twycross Redfort on 1/29/22.
//

#include "QuitCommand.hpp"

QuitCommand::QuitCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr, "QUIT")
{
	_description = "[<Quit message>]";
}

void QuitCommand::execute(User* user, string cmd)
{
	if (cmd.empty())
		sendMessage(user->getFD(), RPL_QUITMSG(user->getUsername(), _serverName, "Client exited"));
	else
		sendMessage(user->getFD(), RPL_QUITMSG(user->getUsername(), _serverName, cmd));
	close(user->getFD());
}
