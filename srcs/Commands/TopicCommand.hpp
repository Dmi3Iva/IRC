#ifndef TOPIC_COMMAND_HPP
#define TOPIC_COMMAND_HPP

#include "../ACommand.hpp"


// https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.4
class TopicCommand: public ACommand
{
public:
	TopicCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr);
	void execute(User* user, string cmd);
};

#endif