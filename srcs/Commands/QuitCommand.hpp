//
// Created by Twycross Redfort on 1/29/22.
//

#ifndef QUITCOMMAND_HPP
#define QUITCOMMAND_HPP

#include "../ACommand.hpp"
#include "../Context.hpp"

class QuitCommand : public ACommand {
public:
	QuitCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr);
	void execute(User* user, string cmd);
};

#endif
