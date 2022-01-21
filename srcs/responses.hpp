#ifndef RESPONSES_HPP
#define RESPONSES_HPP
#include "constants.hpp"
#include <string>

/**
 * REPLIES
 */

#define RPL_MOTDSTART(server, nickname) ":" + server + " 375 " + nickname + " Message of the day - " + DELIMITER

#define RPL_MOTD(server, nickname) \
	":" + server + " 372 " + nickname + " :- Welcome to the Internet Relay Network" + DELIMITER

#define RPL_ENDOFMOTD(server, nickname) ":" + server + " 376 " + nickname + " :End of /MOTD command" + DELIMITER

#define RPL_TOPIC(server, channel, topic) std::string(":") + server + " 332 " + channel + " :" + topic + DELIMITER

#define RPL_NAMREPLY(server, channel, nick) std::string(":") + server + " 353 " + channel + " :" + nick + DELIMITER

#define RPL_ENDOFNAMES(server, channel) \
	std::string(":") + server + " 366 " + channel + " :End of /NAMES list" + DELIMITER

#define RPL_YOUREOPER(server, nickname) \
	std::string(":") + server + " 381 " + nickname + " :You are now an IRC operator" + DELIMITER

/**
 * CUSTOM REPLIES
 */

/**
 *
 * example: ":Guest41!~Guest41@joseon-i28.29p.1d9n3c.IP JOIN :#freenode"
 * <nick>!<user>@<host>
 */
#define JOIN_RPL(nickname, username, host, channel) \
	std::string(":") + nickname + "!" + username + "@" + host + " JOIN " + channel + DELIMITER

/**
 * example: ":Guest41!~Guest41@joseon-i28.29p.1d9n3c.IP PART :#freenode"
 */
#define PART_RPL(nickname, username, host, channel) \
	std::string(":") + nickname + "!" + username + "@" + host + " PART " + channel + DELIMITER

#define RPL_PRIVMSG(nickname, username, host, receiver, message) \
	std::string(":") + nickname + "!" + username + "@" + host + " PRIVMSG " + receiver + std::string(" :") + message + DELIMITER

#define PONG_RPL(msg) std::string("PONG :") + msg + DELIMITER

/**
 * ERRORS
 */

#define ERR_NOSUCHCHANNEL(servername, nickname, channelName) \
	std::string(":") + servername + " 403 " + nickname + " " + channelName + " :No such channel" + DELIMITER

#define ERR_TOOMANYCHANNELS(servername, nickname, channelName) \
	std::string(":") + servername + " 403 " + nickname + " " + channelName + " :You have joined too many channels" + DELIMITER

/**
 * example: ":karma.freenode.net 421 batman PPPP :Unknown command"
 */
#define ERR_UNKNOWNCOMMAND(servername, nickname, command) \
	std::string(":") + servername + " 421 " + nickname + " " + command + " :Unknown command" + DELIMITER

/**
 * example: ":teepee.freenode.net 409 batman :No origin specified"
 */
#define ERR_NOORIGIN(servername, nickname) \
	std::string(":") + servername + " 409 " + nickname + " :No origin specified" + DELIMITER

/**
 * example: ":karma.freenode.net 451 * JOIN :You have not registered."
 */
#define ERR_NOTREGISTERED(servername, nickname, commandName) \
	std::string(":") + servername + " 451 " + nickname + " " + commandName + " :You have not registered." + DELIMITER

/**
 * example:
 * ":karma.freenode.net 461 nickerere NICK :Not enough parameters."
 * ":karma.freenode.net 650 nickerere NICK :<newnick>"
 */
#define ERR_NEEDMOREPARAMS(servername, nickname, commandName) \
	std::string(":") + servername + " 461 " + nickname + " " + commandName + " :Not enough parameters." + DELIMITER

#define ERR_PASSWDMISMATCH(servername, nickname) \
	std::string(":") + servername + " 464 " + nickname + " :Password incorrect" + DELIMITER

#define ERR_CHANNELISFULL(servername, nickname, channelName) \
	std::string(":") + servername + " 471 " + nickname + " " + channelName + " :Cannot join channel (+l)." + DELIMITER

#define ERR_INVITEONLYCHAN(servername, nickname, channelName) \
	std::string(":") + servername + " 473 " + nickname + " " + channelName + " :Cannot join channel (+i)." + DELIMITER

#define ERR_BANNEDFROMCHAN(servername, nickname, channelName) \
	std::string(":") + servername + " 474 " + nickname + " " + channelName + " :Cannot join channel (+b)." + DELIMITER

#define ERR_BADCHANNELKEY(servername, nickname, channelName) \
	std::string(":") + servername + " 475 " + nickname + " " + channelName + " :Cannot join channel (+k)." + DELIMITER

#define ERR_NOOPERHOST(servername, nickname) \
	std::string(":") + servername + " 491 " + nickname + " :No O-lines for your host" + DELIMITER

#define ERR_NOSUCHNICK(servername, nickname) \
	std::string(":") + servername + " 401 " + std::string(nickname) + " PRIVMSG" + DELIMITER

#define ERR_NORECIPIENT(servername, nickname, \
	commandName, description)                 \
	std::string(":") + servername + " 411 " + nickname + std::string(" :No recipient given (") + commandName + ")" + DELIMITER + description + DELIMITER

#define ERR_NOTEXTTOSEND(servername, nickname) \
	std::string(":") + servername + " 412 " + nickname + std::string(" :No text to send") + DELIMITER

#define ERR_TOOMANYTARGETS(servername, nickname, target) \
	std::string(":") + servername + " 407 " + nickname + std::string(target) + " :Duplicate recipients. No message delivered" + DELIMITER

#define ERR_NONICKNAMEGIVEN(servername, nickname) \
	std::string(":") + servername + " 431 " + nickname + " :No nickname given" + DELIMITER

#define ERR_ERRONEUSNICKNAME(servername, nickname, nick) \
	std::string(":") + servername + " 432 " + nickname + " " + nick + " :Erroneus nickname" + DELIMITER

#define ERR_NICKNAMEINUSE(servername, nickname, nick) \
	std::string(":") + servername + " 433 " + nickname + " " + nick + " :Nickname is already in use" + DELIMITER

#define ERR_ALREADYREGISTRED(servername, nickname) \
	std::string(":") + servername + " 462 " + nickname + " :You may not reregister" + DELIMITER

#endif