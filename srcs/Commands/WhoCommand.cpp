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
	} else {
		if (operatorFlag) {
			sendMessage(user->getFD(), ERR_NOSUCHSERVER(_serverName, user->getNickname()));
		} else {
			_performWithUser(user, args[0]);
		}
	}
	sendMessage(user->getFD(), RPL_ENDOFWHO(_serverName, user->getNickname(), args[0]));
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
			string awaystatus = _getAwayStatus(*it);
			sendMessage(user->getFD(), RPL_WHOREPLY(_serverName, user->getNickname(), channel->second->getName(), (*it)->getUsername(), (*it)->getHostname(), (*it)->getNickname(), awaystatus, "@", (*it)->getRealname()));
		}
	} else {
		vector<User*> users = channel->second->getMembers();
		for (vector<User*>::iterator it = users.begin(); it != users.end(); it++) {
			//TO DO - check if user is invisible
			string userStatus = _getUserStatus(channel->second, *it);
			string awaystatus = _getAwayStatus(*it);
			sendMessage(user->getFD(), RPL_WHOREPLY(_serverName, user->getNickname(), channel->second->getName(), (*it)->getUsername(), (*it)->getHostname(), (*it)->getNickname(), awaystatus, userStatus, (*it)->getRealname()));
		}
	}
}

void WhoCommand::_performWithUser(User* user, string userNick)
{
	User *wantedUser = getUserFromArray(userNick);
	if (!wantedUser) {
		sendMessage(user->getFD(), ERR_NOSUCHSERVER(_serverName, userNick));
		return;
	}
	//TO DO - check if user is invisible
	string channel = _getChannelNameWichInUserParticipate(wantedUser->getNickname());
	sendMessage(user->getFD(), RPL_WHOREPLY(_serverName, user->getNickname(), channel, wantedUser->getUsername(), wantedUser->getHostname(), wantedUser->getNickname(), _getAwayStatus(wantedUser), "@", wantedUser->getRealname()));
}

string WhoCommand::_getUserStatus(Channel* channel, User* user)
{
	if (channel->isOperator(user)) {
		return ("@");
	}
	return ("+");
}

string WhoCommand::_getAwayStatus(User* user)
{
	if (user->getIsAway()) {
		return ("G");
	}
	return ("H");
}

string WhoCommand::_getChannelNameWichInUserParticipate(string userNick)
{
	for (channelMap::iterator it = _channelsPtr->begin(); it != _channelsPtr->end(); it++) {
		vector<User*>::const_iterator itUsers = it->second->getMembers().begin();
		vector<User*>::const_iterator itUsersEnd = it->second->getMembers().end();
		while (itUsers != itUsersEnd) {
			if ((*itUsers)->getNickname() == userNick) {
				return (it->second->getName());
			}
			itUsers++;
		}
	}
	return ("*");
}