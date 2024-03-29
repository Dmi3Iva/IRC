#ifndef PRIVATEMESSAGECOMMAND_HPP
#define PRIVATEMESSAGECOMMAND_HPP

#include "../ACommand.hpp"

class PrivateMessageCommand : public ACommand {

public:
	PrivateMessageCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr);
	virtual ~PrivateMessageCommand();

	void execute(User* user, string cmd);

private:
	vector<string> _getReceivers(string& cmd);
	string _constructMessage(string cmd);
	void _sendMessageToReceivers(User* user, vector<string>& receivers, string message);
	bool _isReceiverAlredyGotMessage(list<string>& handledReceivers, string nick);
};

#endif