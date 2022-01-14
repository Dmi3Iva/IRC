#ifndef UTILS
#define UTILS

#include <sstream>
#include <string>
#include <sys/poll.h>
#include <vector>

using std::getline;
using std::istringstream;
using std::string;
using std::vector;

vector<string> ft_split(const string &s, char delim);
pollfd *getPollFdFromFd(int fd);

#endif
