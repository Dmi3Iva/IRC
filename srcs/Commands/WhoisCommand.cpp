#include "WhoisCommand.hpp"

WhoisCommand::WhoisCommand(string serverName, userVector* userPtr, channelMap* channelsPtr)
	: ACommand(serverName, userPtr, channelsPtr, "WHOIS")
{
	_description = "WHOIS <nick>[,<nick>[,...]]";
}

WhoisCommand::~WhoisCommand() { }

void WhoisCommand::execute(User* user, string cmd)
{
	if (!user->isRegistered()) {
		sendMessage(user->getFD(), ERR_NOTREGISTERED(_serverName, user->getNickname(), "WHOIS"));
		return;
	}
	vector<string> args = ft_split(cmd, " ");
	if (args.size() != 1) {
		sendMessage(user->getFD(), ERR_NONICKNAMEGIVEN(_serverName, user->getNickname()));
		return;
	}
	vector<string> nicknames = ft_split(args[0], ",");
	if (nicknames.empty()) {
		sendMessage(user->getFD(), ERR_NONICKNAMEGIVEN(_serverName, user->getNickname()));
		return;
	}
	_presentUsersInformation(user, nicknames);
}

void WhoisCommand::_presentUsersInformation(User* user, vector<string>& nicknames)
{
	(void)user;
	for (vector<string>::iterator it = nicknames.begin(); it != nicknames.end(); it++) {
		User* searchedUser = getUserFromArray(*it);
		if (searchedUser) {
			cout << searchedUser->getNickname() << endl;
			_showUserInfo(user, searchedUser);
		} else {
			sendMessage(user->getFD(), ERR_NOSUCHNICK(_serverName, *it));
		}
	}
}

void WhoisCommand::_showUserInfo(User* user, User* searchedUser)
{
	sendMessage(user->getFD(),
		RPL_WHOISUSER(_serverName, user->getNickname(), searchedUser->getNickname(), searchedUser->getUsername(), searchedUser->getHostname(), searchedUser->getRealname()));
	sendMessage(user->getFD(), RPL_WHOISSERVER(_serverName, user->getNickname(), searchedUser->getNickname(), "ft_IRC"));
	if (searchedUser->isParticipateInChannels()) {
		string channelsInfo = _constructChannelsInfo(searchedUser);
		sendMessage(user->getFD(), RPL_WHOISCHANNELS(_serverName, user->getNickname(), searchedUser->getNickname(), channelsInfo));
	}
	if (searchedUser->isOper()) {
		sendMessage(user->getFD(), RPL_WHOISOPERATOR(_serverName, user->getNickname(), searchedUser->getNickname()));
	}
	if (searchedUser->getIsAway())
		sendMessage(user->getFD(), RPL_AWAY(_serverName, user->getNickname(), searchedUser->getNickname(), searchedUser->getAwayMessage()));
	sendMessage(user->getFD(), RPL_ENDOFWHOIS(_serverName, user->getNickname(), searchedUser->getNickname()));
}

string WhoisCommand::_constructChannelsInfo(User* searchedUser)
{
	map<string, Channel*> userChannels = searchedUser->getChannels();
	cout << "SEGA" << endl;
	string message = "";
	for (map<string, Channel*>::iterator it = userChannels.begin(); it != userChannels.end(); it++) {
		message += it->second->getUserPrefix(searchedUser) + it->second->getName() + " ";
	}
	return (message);
}