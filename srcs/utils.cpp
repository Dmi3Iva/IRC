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
vector<string> ft_split(std::string s, char delim) {
  vector<string> result;
  istringstream iss(s);
  string item;
  while (getline(iss, item, delim)) {
    if (!item.empty())
      result.push_back(item);
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