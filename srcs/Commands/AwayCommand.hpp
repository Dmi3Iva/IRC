#ifndef AWAYCOMMAND_HPP
#define AWAYCOMMAND_HPP

#include "../ACommand.hpp"
#include "../Context.hpp"
#include "../responses.hpp"
#include <iostream>

class ACommand;

class AwayCommand : public ACommand {

public:
	AwayCommand(string serverName, userVector* userPtr, channelMap* channelsPtr);
	virtual ~AwayCommand();

	void execute(User* user, string cmd);
};

#endif