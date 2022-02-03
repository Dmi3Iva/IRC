//
// Created by Twycross Redfort on 1/22/22.
//

#include "PassCommand.hpp"

PassCommand::PassCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr, const string& serverPassword)
	: ACommand(serverName, usersPtr, channelsPtr, "PASS")
	, _serverPassword(serverPassword)
{
}

void PassCommand::execute(User* user, string cmd)
{
	if (user->isAuthenticated())
		user->appendBuffer(ERR_ALREADYREGISTRED(_serverName, user->getNickname()));
//		sendMessage(user->getFD(), ERR_ALREADYREGISTRED(_serverName, user->getNickname()));
	else if (cmd[0] == ':' && &cmd[1] == _serverPassword)
		user->setIsAuthenticated(true);
	else {
		user->appendBuffer(ERR_NEEDMOREPARAMS(_serverName, (user->getNickname().empty() ? "*" : user->getNickname()), _name));
//		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, (user->getNickname().empty() ? "*" : user->getNickname()), _name));
		close(user->getFD());
	}
}
