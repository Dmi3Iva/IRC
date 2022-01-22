#include "WhoCommand.hpp"

WhoCommand::WhoCommand(string serverName, userVector* userPtr, channelMap* channelsPtr)
	: ACommand(serverName, userPtr, channelsPtr, "WHO")
{
	_description = "WHO [<name> [<o>]]";
}

WhoCommand::~WhoCommand() { }

void WhoCommand::execute(User* user, string cmd)
{
	if (!user->isRegistered()) {
		sendMessage(user->getFD(), ERR_NOTREGISTERED(_serverName, user->getNickname(), "WHO"));
		return;
	}
	vector<string> args = ft_split(cmd, " ");
	if (args.size() < 1 || 2 < args.size()) {
		sendMessage(user->getFD(), ERR_NOSUCHSERVER(_serverName, user->getNickname()));
		return;
	}
	bool operatorFlag = false;
	if (args.size() == 2) {
		if (args[1] == "o") {
			operatorFlag = true;
		} else {
			sendMessage(user->getFD(), ERR_NOSUCHSERVER(_serverName, user->getNickname()));
			return;
		}
	}
	if (isChannel(args[0])) {
		_performWithChannel(user, args[0], operatorFlag);
	}
}

void WhoCommand::_performWithChannel(User* user, string channelName, bool operatorFlag)
{
	channelMap::iterator channel = _channelsPtr->find(channelName);
	if (channel == _channelsPtr->end()) {
		sendMessage(user->getFD(), ERR_NOSUCHSERVER(_serverName, user->getNickname()));
		return;
	}
	if (operatorFlag) {
		vector<User*> users = channel->second->getOperators();
		for (vector<User*>::iterator it = users.begin(); it != users.end(); it++) {
			//TO DO - check if user is invisible
			string awaystatus = getAwayStatus(*it);
			sendMessage(user->getFD(), RPL_WHOREPLY(_serverName, user->getNickname(), channel->second->getName(), (*it)->getUsername(), (*it)->getHostname(), (*it)->getNickname(), awaystatus, "@", (*it)->getRealname()));
		}
	} else {
		vector<User*> users = channel->second->getMembers();
		for (vector<User*>::iterator it = users.begin(); it != users.end(); it++) {
			//TO DO - check if user is invisible
			string userStatus = getUserStatus(channel->second, *it);
			string awaystatus = getAwayStatus(*it);
			sendMessage(user->getFD(), RPL_WHOREPLY(_serverName, user->getNickname(), channel->second->getName(), (*it)->getUsername(), (*it)->getHostname(), (*it)->getNickname(), awaystatus, userStatus, (*it)->getRealname()));
		}
	}
	sendMessage(user->getFD(), RPL_ENDOFWHO(_serverName, user->getNickname(), channelName));
}

string WhoCommand::getUserStatus(Channel* channel, User* user)
{
	if (channel->isOperator(user)) {
		return ("@");
	}
	return ("+");
}

string WhoCommand::getAwayStatus(User* user)
{
	if (user->getIsAway()) {
		return ("G");
	}
	return ("H");
}
