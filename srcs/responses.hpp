//
// Created by Shandy Mephesto on 1/14/22.
//

#ifndef IRC_RESPONSES_HPP
#define IRC_RESPONSES_HPP
#include <string>

#define RPL_WELCOME(nickname, username, hostname)                                                                      \
  std::string(":") + hostname + std::string(" 001 ") + nickname + std::string(" :Welcome to the Internet Relay ") +    \
      std::string("Network ") + nickname + std::string("!") + username + std::string("@") + hostname +                 \
      std::string("\r\n")

#endif // IRC_RESPONSES_HPP
