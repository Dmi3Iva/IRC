#ifndef KICKCOMMAND_HPP
#define KICKCOMMAND_HPP

#include "../ACommand.hpp"
#include "../Context.hpp"
#include "../responses.hpp"
#include <iostream>

class ACommand;

class KickCommand : public ACommand {

public:
	KickCommand(string serverName, userVector* userPtr, channelMap* channelsPtr);
	virtual ~KickCommand();

	void execute(User* user, string cmd);

private:
	string _constructMessageAndEraseFromCmd(string &cmd);
	void _startKicking(User *user, vector<string> &channels, vector<string> &users);
	void _kickUsersFromChannel(User *user, Channel *channel, vector<string> &users);
};

#endif