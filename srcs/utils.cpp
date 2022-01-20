//
// Created by Shandy Mephesto on 1/13/22.
//

#include "utils.hpp"

/**
 * TODO::
 * @param s
 * @param delim
 * @return
 */
vector<string> ft_split(string s, const string &delim) {
  vector<string> result;
  size_t pos = 0;
  string token;
  while ((pos = s.find(delim)) != std::string::npos) {
    token = s.substr(0, pos);
    if (!token.empty())
      result.push_back(token);
    s.erase(0, pos + delim.length());
  }
  return result;
}

/**
 * Create pollFdPointer with pollFd structure
 * @param fd
 * @return
 */
pollfd *getPollFdFromFd(int fd) {
  pollfd *pollfdPtr = new pollfd[1];
  pollfdPtr[0].fd = fd;
  pollfdPtr[0].events = POLLIN | POLLOUT;
  pollfdPtr[0].revents = 0;
  return pollfdPtr;
}

std::string &ltrim(std::string &str, const std::string &chars) {
  str.erase(0, str.find_first_not_of(chars));
  return str;
}

std::string &rtrim(std::string &str, const std::string &chars) {
  str.erase(str.find_last_not_of(chars) + 1);
  return str;
}

std::string &trim(std::string &str, const std::string &chars) { return ltrim(rtrim(str, chars), chars); }

struct pollfd fillPollfd(int sd, short events) {
  struct pollfd fd;
  fd.fd = sd;
  fd.events = events;
  fd.revents = 0;
  return fd;
}
