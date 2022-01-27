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
			if ((*it)->isInvisible())
				continue;
			string awaystatus = _getAwayStatus(*it);
			string serverstatus = _getUserStatusOnServer(user);
			sendMessage(user->getFD(),
				RPL_WHOREPLY(_serverName, user->getNickname(), channel->second->getName(), (*it)->getUsername(), (*it)->getHostname(), (*it)->getNickname(), awaystatus,
					serverstatus, "@", (*it)->getRealname()));
		}
	} else {
		vector<User*> users = channel->second->getMembers();
		for (vector<User*>::iterator it = users.begin(); it != users.end(); it++) {
			if ((*it)->isInvisible())
				continue;
			string userStatus = channel->second->getUserPrefix(*it);
			string awaystatus = _getAwayStatus(*it);
			string serverstatus = _getUserStatusOnServer(user);
			sendMessage(user->getFD(),
				RPL_WHOREPLY(_serverName, user->getNickname(), channel->second->getName(), (*it)->getUsername(), (*it)->getHostname(), (*it)->getNickname(), awaystatus,
					serverstatus, userStatus, (*it)->getRealname()));
		}
	}
}

void WhoCommand::_performWithUser(User* user, string userNick)
{
	User* wantedUser = getUserFromArray(userNick);
	if (!wantedUser) {
		sendMessage(user->getFD(), ERR_NOSUCHSERVER(_serverName, userNick));
		return;
	}
	if (wantedUser->isInvisible())
		return;
	string channel = _getChannelNameWichInUserParticipate(wantedUser);
	string serverstatus = _getUserStatusOnServer(user);
	sendMessage(user->getFD(),
		RPL_WHOREPLY(_serverName, user->getNickname(), channel, wantedUser->getUsername(), wantedUser->getHostname(), wantedUser->getNickname(), _getAwayStatus(wantedUser),
			serverstatus, "@", wantedUser->getRealname()));
}

string WhoCommand::_getAwayStatus(User* user)
{
	if (user->getIsAway()) {
		return ("G");
	}
	return ("H");
}

string WhoCommand::_getUserStatusOnServer(User* user)
{
	if (user->isOper())
		return ("*");
	return ("");
}

string WhoCommand::_getChannelNameWichInUserParticipate(User *user)
{
	for (channelMap::iterator it = _channelsPtr->begin(); it != _channelsPtr->end(); it++) {
		if (it->second->isUserMember(user))
			return (it->second->getName());
	}
	return ("*");
}