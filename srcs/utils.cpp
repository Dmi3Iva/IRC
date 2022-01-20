//
// Created by Shandy Mephesto on 1/13/22.
//

#include "utils.hpp"

/**
 * split string with delimiter
 * @param s source string
 * @param delim delimiter string
 * @return vector of strings
 */

vector<string> ft_split (string s, const string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }
    res.push_back (s.substr (pos_start));
    return res;
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

int isChannelName(string channelName) {
  if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&'))
    return 0;
  return 1;
}

bool isPUserInVector(User *pUser, vector<User *> userList) {
  for (vector<User *>::iterator it = userList.begin(), ite = userList.end(); it != ite; ++it) {
    if ((*it)->getNickname() == pUser->getNickname()) {
      return 1;
    }
  }
  return 0;
}
