//
// Created by Shandy Mephesto on 1/14/22.
//

#ifndef IRC_RESPONSES_HPP
#define IRC_RESPONSES_HPP
#include "constants.hpp"
#include <string>


#define RPL_MOTDSTART(server, nickname)     ":" + server + " 375 " + nickname + " Message of the day - \n"
#define RPL_MOTD(server, nickname)          ":" + server + " 372 " + nickname + " :- " + "Welcome to the Internet Relay Network\n"
#define RPL_ENDOFMOTD(server, nickname)     ":" + server + " 376 " + nickname + " :End of /MOTD command\n"


#endif // IRC_RESPONSES_HPP
