#include "AwayCommand.hpp"

AwayCommand::AwayCommand(string serverName, userVector* userPtr, channelMap* channelsPtr)
	: ACommand(serverName, userPtr, channelsPtr, "AWAY")
{
	_description = "AWAY [message]]";
}

AwayCommand::~AwayCommand() { }

void AwayCommand::execute(User* user, string cmd)
{
	eraseSpacesInFront(cmd);
	if (cmd.empty()) {
		user->setIsAway(false);
		user->setAwayMessage("");
		user->appendBuffer(RPL_UNAWAY(_serverName, user->getNickname()));
//		sendMessage(user->getFD(), RPL_UNAWAY(_serverName, user->getNickname()));
	} else {
		cmd.erase(remove(cmd.begin(), cmd.end(), ':'), cmd.end());
		user->setIsAway(true);
		user->setAwayMessage(cmd);
		user->appendBuffer(RPL_NOWAWAY(_serverName, user->getNickname()));
//		sendMessage(user->getFD(), RPL_NOWAWAY(_serverName, user->getNickname()));
	}
}