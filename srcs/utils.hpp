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

vector<string> ft_split(string s, const string &delim);
pollfd *getPollFdFromFd(int fd);

std::string &ltrim(std::string &str, const std::string &chars = "\t\n\v\f\r ");
std::string &rtrim(std::string &str, const std::string &chars = "\t\n\v\f\r ");
std::string &trim(std::string &str, const std::string &chars = "\t\n\v\f\r ");
#endif
