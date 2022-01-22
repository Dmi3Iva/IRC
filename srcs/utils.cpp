//
// Created by Shandy Mephesto on 1/13/22.
//

#include "utils.hpp"

std::vector<string> ft_split(const string& s, const string& delimiter) { return ft_split<vector<string> >(s, delimiter); }

pollfd* getPollFdFromFd(int fd)
{
	pollfd* pollfdPtr = new pollfd[1];
	pollfdPtr[0].fd = fd;
	pollfdPtr[0].events = POLLIN | POLLOUT;
	pollfdPtr[0].revents = 0;
	return pollfdPtr;
}

std::string ltrim(std::string str, const std::string& chars)
{
	str.erase(0, str.find_first_not_of(chars));
	return str;
}

std::string rtrim(std::string str, const std::string& chars)
{
	str.erase(str.find_last_not_of(chars) + 1);
	return str;
}

std::string trim(std::string str, const std::string& chars) { return ltrim(rtrim(str, chars), chars); }

void sendMessage(int fd, string msg)
{
	cout << "sending message to " << fd << ": " << msg << endl;
	send(fd, msg.c_str(), msg.size(), 0);
}

int isChannelName(string channelName)
{
	if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&'))
		return 0;
	return 1;
}

void eraseSpacesInFront(string& cmd)
{
	int i = 0;
	while (cmd[i] && cmd[i] == ' ')
		i++;
	cmd.erase(0, i);
}
struct pollfd fillPollfd(int sd, short events)
{
	struct pollfd fd;
	fd.fd = sd;
	fd.events = events;
	fd.revents = 0;
	return fd;
}
