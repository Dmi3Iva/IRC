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
	void _handleOFlag(User* user, Channel* channel, bool isPlus, vector<string> optionalArguments);
	void _handleCommonFlag(User* user, Channel* channel, bool isPlus, void (Channel::*setValue)(bool), bool isValue, string key, string& changes);
	void _handleLFlag(User* user, Channel* channel, string argument, bool isPlus, string& changes);
	void _handleBFLag(User* user, Channel* channel, bool isPlus, string argument);
	void _handleVFlag(User* user, Channel* channel, string argument, bool isPlus);
	void _handleKFlag(User* user, Channel* channel, string argument, string& changes, bool isPlus);
};

#endif
