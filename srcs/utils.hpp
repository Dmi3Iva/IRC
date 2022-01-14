#ifndef IRC_UTILS_HPP
#define IRC_UTILS_HPP
#include <sstream>
#include <string>
#include <sys/poll.h>
#include <vector>

using std::getline;
using std::istringstream;
using std::string;
using std::vector;

vector<string> ft_split(const std::string &s, char delim);
pollfd *getPollFdFromFd(int fd);

#endif // IRC_UTILS_HPP
