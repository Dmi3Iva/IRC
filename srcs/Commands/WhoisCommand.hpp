#ifndef WHOISCOMMAND_HPP
#define WHOISCOMMAND_HPP

#include "../ACommand.hpp"
#include "../Context.hpp"
#include "../responses.hpp"
#include <iostream>

class ACommand;

class WhoisCommand : public ACommand {

public:
	WhoisCommand(string serverName, userVector* userPtr, channelMap* channelsPtr);
	virtual ~WhoisCommand();

	void execute(User* user, string cmd);
};

#endif