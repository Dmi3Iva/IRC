#ifndef NOTICECOMMAND_HPP
#define NOTICECOMMAND_HPP

#include "../ACommand.hpp"

class NoticeCommand : public ACommand {

public:
	NoticeCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr);
	virtual ~NoticeCommand();

	void execute(User* user, string cmd);

private:
	vector<string> _getReceivers(string& cmd);
	string _constructMessage(string cmd);
	void _sendMessageToReceivers(User* user, vector<string>& receivers, string message);
	bool _isReceiverAlredyGotMessage(list<string>& handledReceivers, string nick);
};

#endif