#include "OperCommand.hpp"

OperCommand::OperCommand(string serverName, ACommand::userVector* usersPtr, ACommand::channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr, "OPER")
{
	_description = "<user> <password>";
}

void OperCommand::execute(User* user, string cmd)
{
	vector<string> args = ft_split(cmd, " ");

	if (args.size() < 2) {
		sendMessage(user->getFD(), ERR_NEEDMOREPARAMS(_serverName, user->getNickname(), _name));
		return;
	}
	if (!user->isRegistered()) {
		sendMessage(user->getFD(), ERR_NOTREGISTERED(_serverName, (user->getNickname().empty() ? std::string("*") : user->getNickname()), _name));
		return;
	}
	if (string(PASS_TO_OPER) == "") {
		sendMessage(user->getFD(), ERR_NOOPERHOST(_serverName, user->getNickname()));
		return;
	}
	if (args.size() > 2 || args[1] != PASS_TO_OPER) {
		sendMessage(user->getFD(), ERR_PASSWDMISMATCH(_serverName, user->getNickname()));
		return;
	}
	User* target = getUserFromArray(args[0]);
	if (target) {
		if (!target->isOper()) {
			target->setIsOper(true);
			sendMessage(target->getFD(), RPL_YOUREOPER(_serverName, target->getNickname()));
			for (userVector::iterator it = _usersPtr->begin(); it != _usersPtr->end(); ++it) {
				sendMessage((*it)->getFD(), MODE_RPL(user->getNickname(), user->getUsername(), user->getHostname(), target->getNickname(), "+o :" + target->getNickname()));
			}
		}
		return;
	}
	sendMessage(user->getFD(), ERR_NOSUCHNICK(_serverName, user->getNickname()));
}
