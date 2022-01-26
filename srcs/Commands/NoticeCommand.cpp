#include "NoticeCommand.hpp"

NoticeCommand::NoticeCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr)
	: ACommand(serverName, usersPtr, channelsPtr, "NOTICE")
{
	_description = "NOTICE <nickname> <text>";
}

NoticeCommand::~NoticeCommand() { }

void NoticeCommand::execute(User* user, string cmd)
{
	if (!user->isRegistered()) {
		return;
	}

	vector<string> receivers = _getReceivers(cmd);
	if (receivers.size() == 0 || receivers[0] == "" || receivers[0].c_str()[0] == ':') {
		return;
	}

	string message = _constructMessage(cmd);
	if (message.size() == 0 || message == "") {
		return;
	}
	_sendMessageToReceivers(user, receivers, message);
}

string NoticeCommand::_constructMessage(string cmd)
{
	size_t receiversLen = cmd.find(' ');
	if (receiversLen == string::npos) {
		return ("");
	}
	cmd.erase(0, receiversLen);
	eraseSpacesInFront(cmd);
	cmd.erase(remove(cmd.begin(), cmd.end(), ':'), cmd.end());
	return (cmd);
}

vector<string> NoticeCommand::_getReceivers(string& cmd)
{
	vector<string> receivers;

	eraseSpacesInFront(cmd);
	size_t receiversLen = cmd.find(' ');
	if (receiversLen == string::npos) {
		return (receivers);
	}
	string receiversStr = cmd.substr(0, receiversLen);
	receivers = ft_split(receiversStr, ",");
	return (receivers);
}

void NoticeCommand::_sendMessageToReceivers(User* user, vector<string>& receivers, string message)
{
	list<string> handledReceivers;

	for (vector<string>::iterator it = receivers.begin(); it < receivers.end(); it++) {
		if (isChannel(*it)) {
			channelMap::iterator channel = _channelsPtr->find(*it);
			if (channel != _channelsPtr->end()) {
				if (!_isReceiverAlredyGotMessage(handledReceivers, channel->second->getName())) {
					channel->second->sendToAllChannelMembers(RPL_PRIVMSG(user->getNickname(), user->getUsername(), user->getUsername(), *it, message), user);
					handledReceivers.push_back(channel->second->getName());
				}
			}
		} else {
			User* userReceiver = this->getUserFromArray(*it);
			if (userReceiver) {
				if (!_isReceiverAlredyGotMessage(handledReceivers, userReceiver->getNickname())) {
					sendMessage(userReceiver->getFD(), RPL_PRIVMSG(user->getNickname(), user->getUsername(), user->getUsername(), *it, message));
					handledReceivers.push_back(userReceiver->getNickname());
				}
			}
		}
	}
}

bool NoticeCommand::_isReceiverAlredyGotMessage(list<string>& handledReceivers, string nick)
{
	for (list<string>::iterator it = handledReceivers.begin(); it != handledReceivers.end(); it++) {
		if (*it == nick) {
			return (true);
		}
	}
	return (false);
}