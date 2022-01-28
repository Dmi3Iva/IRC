//
// Created by Twycross Redfort on 1/24/22.
//

#ifndef NAMESCOMMAND_HPP
#define NAMESCOMMAND_HPP

#include "../ACommand.hpp"
/**
 * https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.5
 */
class NamesCommand : public ACommand {
public:
	NamesCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr);
	void execute(User* user, string cmd);

private:
	void _sendListOfChannelMembers(User* user, Channel* channel);
};

#endif
