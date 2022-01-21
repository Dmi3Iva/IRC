#include "NickCommand.hpp"

NickCommand::NickCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr)
{
	_name = "NICK";
	_description = "NICK <nickname> [ <hopcount> ]";
}

void NickCommand::execute(User* user, string cmd)
{
	cout << "Execute command " << cmd << endl;
	user->setNickname(cmd);
	if (!user->getNickname().empty() && !user->getRealname().empty() && !user->getUsername().empty()) {
		user->setIsRegistered(true);
		string msg = //
			RPL_MOTDSTART(_serverName, user->getNickname()) //
			RPL_MOTD(_serverName, user->getNickname()) //
			RPL_ENDOFMOTD(_serverName, user->getNickname());
		sendMessage(user->getFD(), msg);
	}
}
