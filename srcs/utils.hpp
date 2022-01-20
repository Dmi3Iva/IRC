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
using std::vector;

class User;

vector<string> ft_split(string s, const string &delim);
pollfd *getPollFdFromFd(int fd);
int isChannelName(string channelName);

std::string &ltrim(std::string &str, const std::string &chars = "\t\n\v\f\r ");
std::string &rtrim(std::string &str, const std::string &chars = "\t\n\v\f\r ");
std::string &trim(std::string &str, const std::string &chars = "\t\n\v\f\r ");

template <class Container> void fullDeleteContainer(Container &c) {
  for (typename Container::iterator it = c.begin(), ite = c.end(); it != ite; ++it) {
    delete &(*it);
  }
  c.clear();
}

template <class MapContainer> void fullDeleteMapContainer(MapContainer &c) {
  for (typename MapContainer::iterator it = c.begin(), ite = c.end(); it != ite; ++it) {
    delete (it->second);
  }
  c.clear();
}

bool isPUserInVector(User *pUser, vector<User *> userList);

#endif
