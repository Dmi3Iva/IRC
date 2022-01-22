#ifndef WHOCOMMAND_HPP
#define WHOCOMMAND_HPP

#include "../ACommand.hpp"
#include "../Context.hpp"
#include "../responses.hpp"
#include <iostream>

class ACommand;

class WhoCommand : public ACommand {

public:
	WhoCommand(string serverName, userVector* userPtr, channelMap* channelsPtr);
	virtual ~WhoCommand();

	void execute(User* user, string cmd);

private:
	void _performWithChannel(User* user, string channelName, bool operatorFlag);
	string getUserStatus(Channel *channel, User *user);
	string getAwayStatus(User *user);
};

#endif