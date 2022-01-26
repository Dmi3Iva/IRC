//
// Created by Twycross Redfort on 1/22/22.
//

#ifndef PASSCOMMAND_HPP
#define PASSCOMMAND_HPP

#include "../ACommand.hpp"

class PassCommand : public ACommand {
private:
	const string& _serverPassword;

public:
	PassCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr, const string& serverPassword);
	void execute(User* user, string cmd);
};

#endif
