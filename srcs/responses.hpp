//
// Created by Shandy Mephesto on 1/14/22.
//

#ifndef IRC_RESPONSES_HPP
#define IRC_RESPONSES_HPP

#define RPL_WELCOME(nickname, username, host)                                                                          \
  ":#host 001 #nickname :Welcome to the Internet Relay Network \
!#username@#host"

#endif // IRC_RESPONSES_HPP
