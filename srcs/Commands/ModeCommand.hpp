#ifndef MODECOMMAND_HPP
#define MODECOMMAND_HPP

#include "../ACommand.hpp"

class ModeCommand : public ACommand {
public:
	ModeCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr);
	void execute(User* user, string cmd);

private:
	void _executeChannelMod(User* user, Channel* channel, string arguments);
	void _executeUserMod(User* user, User* userTarget, string arguments);
	void _sendMessageToUserAndHisChannels(User* userTarget, string message);
};

#endif
