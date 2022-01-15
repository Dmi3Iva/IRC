#ifndef RESPONSES_HPP
#define RESPONSES_HPP
#include "constants.hpp"
#include <string>

/**
 * REPLIES
 */

#define RPL_MOTDSTART(server, nickname) ":" + server + " 375 " + nickname + " Message of the day - " + DELIMITER

#define RPL_MOTD(server, nickname)                                                                                     \
  ":" + server + " 372 " + nickname + " :- " + "Welcome to the Internet Relay Network" + DELIMITER

#define RPL_ENDOFMOTD(server, nickname) ":" + server + " 376 " + nickname + " :End of /MOTD command" + DELIMITER

#define RPL_TOPIC(server, channel, topic) std::string(":") + server + " 332 " + channel + " :" + topic + DELIMITER

#define RPL_NAMREPLY(server, channel, nick) std::string(":") + server + " 353 " + channel + " :" + nick + DELIMITER

#define RPL_ENDOFNAMES(server, channel)                                                                                \
  std::string(":") + server + " 366 " + channel + " :End of /NAMES list" + DELIMITER

/**
 * CUSTOM REPLIES
 */

/**
 *
 * example: ":Guest41!~Guest41@joseon-i28.29p.1d9n3c.IP JOIN :#freenode"
 * <nick>!<user>@<host>
 */
#define JOIN_REPL(nickname, username, host, channel)                                                                   \
  std::string(":") + nickname + "!" + username + "@" + host + " JOIN " + channel + DELIMITER

/**
 * example: ":Guest41!~Guest41@joseon-i28.29p.1d9n3c.IP PART :#freenode"
 */
#define PART_REPL(nickname, username, host, channel)                                                                   \
  std::string(":") + nickname + "!" + username + "@" + host + " PART " + channel + DELIMITER

/**
 * ERRORS
 */

#define ERR_NOSUCHCHANNEL(servername, nickname, channelName)                                                           \
  std::string(":") + servername + " 403 " + nickname + " " + channelName + ":No such channel" + DELIMITER

#define ERR_TOOMANYCHANNELS(servername, nickname, channelName)                                                         \
  std::string(":") + servername + " 403 " + nickname + " " + channelName + ":You have joined too many channels" +      \
      DELIMITER
/**
 * example:
 * ":karma.freenode.net 461 nickerere NICK :Not enough parameters."
 * ":karma.freenode.net 650 nickerere NICK :<newnick>"
 */
#define ERR_NEEDMOREPARAMS(servername, nickname, commandName)                                                          \
  std::string(":") + servername + " 461 " + nickname + " " + commandName + ":Not enough parameters." + DELIMITER

/**
 * example: ":karma.freenode.net 451 * JOIN :You have not registered."
 */
#define ERR_NOTREGISTERED(servername, nickname, commandName)                                                           \
  std::string(":") + servername + " 451 " + nickname + " " + commandName + ":You have not registered." + DELIMITER

#define ERR_CHANNELISFULL(servername, nickname, channelName)                                                           \
  std::string(":") + servername + " 471 " + nickname + " " + channelName + ":Cannot join channel (+l)." + DELIMITER

#define ERR_INVITEONLYCHAN(servername, nickname, channelName)                                                          \
  std::string(":") + servername + " 473 " + nickname + " " + channelName + ":Cannot join channel (+i)." + DELIMITER

#define ERR_BANNEDFROMCHAN(servername, nickname, channelName)                                                          \
  std::string(":") + servername + " 474 " + nickname + " " + channelName + ":Cannot join channel (+b)." + DELIMITER

#define ERR_BADCHANNELKEY(servername, nickname, channelName)                                                           \
  std::string(":") + servername + " 475 " + nickname + " " + channelName + ":Cannot join channel (+k)." + DELIMITER

#endif