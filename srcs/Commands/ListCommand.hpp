//
// Created by Twycross Redfort on 1/24/22.
//

#ifndef LISTCOMMAND_HPP
#define LISTCOMMAND_HPP

#include "../ACommand.hpp"
/**
 * https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.6
 */
class ListCommand : public ACommand {
public:
	ListCommand(string serverName, userVector* usersPtr, channelMap* channelsPtr);
	void execute(User* user, string cmd);

private:
	void _sendListOfChannelTopics(User* user, Channel* channel);
	int _getNumberOfVisibleUsers(vector<User*>& users);
};

#endif
