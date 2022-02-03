#ifndef UTILS_HPP
#define UTILS_HPP

#include "User.hpp"
#include <sstream>
#include <string>
#include <sys/poll.h>
#include <vector>

using std::getline;
using std::istringstream;
using std::string;
using std::stringstream;
using std::vector;

class User;

/**
 * split string with delimiter. It skips empty strings
 * @param s source string
 * @param delim delimiter string
 * @return vector of strings
 */
template <class Container> Container ft_split(const string& s, const string& delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	Container res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		if (!token.empty())
			res.push_back(token);
	}
	token = s.substr(pos_start);
	if (!token.empty())
		res.push_back(token);
	return res;
}

std::vector<string> ft_split(const string& s, const string& delimiter);

/**
 * Create pollFdPointer with pollFd structure
 * @param fd
 * @return
 */
pollfd* getPollFdFromFd(int fd);

int isChannelName(string channelName);

std::string ltrim(std::string str, const std::string& chars = "\t\n\v\f\r ");
std::string rtrim(std::string str, const std::string& chars = "\t\n\v\f\r ");
std::string trim(std::string str, const std::string& chars = "\t\n\v\f\r ");

void sendMessage(int fd, string msg);

template <class Container> void fullDeleteContainer(Container& c)
{
	for (typename Container::iterator it = c.begin(), ite = c.end(); it != ite; ++it) {
		delete &(*it);
	}
	c.clear();
}

template <class MapContainer> void fullDeleteMapContainer(MapContainer& c)
{
	for (typename MapContainer::iterator it = c.begin(), ite = c.end(); it != ite; ++it) {
		delete (it->second);
	}
	c.clear();
}

void eraseSpacesInFront(string& cmd);

struct pollfd fillPollfd(int sd, short events);

string getListOfNicknames(const vector<User*> users, string separator);

bool isChannel(string receiver);

template <class Container> typename Container::value_type getPopFront(Container c)
{
	if (c.empty())
		return typename Container::value_type();
	typename Container::value_type result = c.front();
	c.erase(c.begin());
	return result;
}

bool isMaskMatch(const string& str, const string& mask);

bool isMaskMatch(const char* str, const char* mask);

bool StarCmp(const char* str, const char* mask);

#endif
