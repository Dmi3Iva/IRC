#ifndef INVITECOMMAND_HPP
#define INVITECOMMAND_HPP

#include "../ACommand.hpp"
#include "../Context.hpp"
#include "../responses.hpp"
#include <iostream>

class ACommand;

class InviteCommand : public ACommand {

public:
	InviteCommand(string serverName, userVector* userPtr, channelMap* channelsPtr);
	virtual ~InviteCommand();

	void execute(User* user, string cmd);

private:
	void _sendInvite(User *user, Channel *channel, User *userToInvite);
};

#endif