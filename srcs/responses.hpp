#ifndef RESPONSES_HPP
#define RESPONSES_HPP

#include "constants.hpp"
#include <string>

/**
 * REPLIES
 */

#define RPL_CHANNELMODEIS()

#define RPL_QUITMSG(username, ip, message) "ERROR :Closing link: (~" + username.substr(0, 9) + "@" + ip + ") [" + message + "]" + DELIMITER

#define RPL_MOTDSTART(server, nickname) ":" + server + " 375 " + nickname + " Message of the day - " + DELIMITER

#define RPL_MOTD(server, nickname) ":" + server + " 372 " + nickname + " :- Welcome to the Internet Relay Network" + DELIMITER

#define RPL_ENDOFMOTD(server, nickname) ":" + server + " 376 " + nickname + " :End of /MOTD command" + DELIMITER

#define RPL_TOPIC(server, channel, topic) std::string(":") + server + " 332 " + channel + " :" + topic + DELIMITER

#define RPL_NAMREPLY(server, channel, provideWho, nick) std::string(":") + server + " 353 " + provideWho + " " + channel + " :" + nick + DELIMITER

#define RPL_ENDOFNAMES(server, channel) std::string(":") + server + " 366 " + channel + " :End of /NAMES list" + DELIMITER

#define RPL_WHOREPLY(server, nickname, channel, username, host, userNick, awaystatus, serverstatus, operator, realname)                                                            \
	std::string(":") + server + " 352 " + nickname + " " + channel + " " + username + " " + host + " " + server + " " + userNick + " " + awaystatus + serverstatus                 \
		+ operator+ " :0 " + realname + DELIMITER

#define RPL_ENDOFWHO(server, nickname, name) std::string(":") + server + " 315 " + nickname + " " + name + " :End of /WHO list" + DELIMITER

#define RPL_YOUREOPER(server, nickname) std::string(":") + server + " 381 " + nickname + " :You are now an IRC operator" + DELIMITER

#define RPL_UNAWAY(server, nickname) std::string(":") + server + " 305 " + nickname + " :You are no longer marked as being away" + DELIMITER

#define RPL_NOWAWAY(server, nickname) std::string(":") + server + " 306 " + nickname + " :You have been marked as being away" + DELIMITER

#define RPL_AWAY(server, nickname, receivernick, message) std::string(":") + server + " 301 " + nickname + " " + receivernick + " :" + message + DELIMITER

#define RPL_WHOISUSER(server, nickname, nick, user, host, name) std::string(":") + server + " 311 " + nickname + " " + nick + " " + user + " " + host + " * :" + name + DELIMITER

#define RPL_WHOISSERVER(server, nickname, nick, serverinfo) std::string(":") + server + " 312 " + nickname + " " + nick + " " + server + " :" + serverinfo + DELIMITER

#define RPL_ENDOFWHOIS(server, nickname, nick) std::string(":") + server + " 318 " + nickname + " " + nick + " :End of WHOIS list" + DELIMITER

#define RPL_WHOISCHANNELS(server, nickname, nick, channelsinfo) std::string(":") + server + " 319 " + nickname + " " + nick + " :" + channelsinfo + DELIMITER

#define RPL_WHOISOPERATOR(server, nickname, nick) std::string(":") + server + " 313 " + nickname + " " + nick + " :is an IRC operator" + DELIMITER

#define RPL_INVITING(server, nickname, channel, nick) std::string(":") + server + " 341 " + nickname + " " + channel + " :" + nick + DELIMITER

/**
 * CUSTOM REPLIES
 */

/**
 *
 * example: ":Guest41!~Guest41@joseon-i28.29p.1d9n3c.IP JOIN :#freenode"
 * <nick>!<user>@<host>
 */
#define JOIN_RPL(nickname, username, host, channel) std::string(":") + nickname + "!" + username + "@" + host + " JOIN " + channel + DELIMITER

/**
 * example: ":Guest41!~Guest41@joseon-i28.29p.1d9n3c.IP PART :#freenode"
 */
#define PART_RPL(nickname, username, host, channel) std::string(":") + nickname + "!" + username + "@" + host + " PART " + channel + DELIMITER

#define RPL_PRIVMSG(nickname, username, host, receiver, message)                                                                                                                   \
	std::string(":") + nickname + "!" + username + "@" + host + " PRIVMSG " + receiver + std::string(" :") + message + DELIMITER

#define PONG_RPL(msg) std::string("PONG :") + msg + DELIMITER

#define RPL_KICK(nickname, username, host, channel, kickeddude, message)                                                                                                           \
	std::string(":") + nickname + "!" + username + "@" + host + " KICK " + channel + " " + kickeddude + std::string(" :") + message + DELIMITER

/**
 * example: ":Guest36552!~uss@joseon-i28.29p.1d9n3c.IP MODE Guest36552 :-w"
 * :lara!Adium@* MODE #new :+o :qwe
 * :Guest19803!~uss@joseon-i28.29p.1d9n3c.IP MODE #nnnn4 +l :10
 * :Guest50!~Guest50@joseon-i28.29p.1d9n3c.IP MODE #myfreenode +o :qwerty
 */
#define MODE_RPL(nickname, username, host, targetNickname, changes)                                                                                                                \
	std::string(":") + nickname + "!" + username + "@" + host + " MODE " + targetNickname + " " + changes + DELIMITER

#define INVITE_RPL(nickname, username, host, receiver, channel)                                                                                                                    \
	std::string(":") + nickname + "!" + username + "@" + host + " INVITE " + receiver + std::string(" :") + channel + DELIMITER

//#define MODE_CHANNEL_RPL(nickname, username, host, targetNickname, changes)                                                                                                        \
//	std::string(":") + nickname + "!" + username + "@" + host + " MODE " + targetNickname + " :" + changes + DELIMITER

/**
 * ERRORS
 */

#define ERR_NOSUCHCHANNEL(servername, nickname, channelName) std::string(":") + servername + " 403 " + nickname + " " + channelName + " :No such channel" + DELIMITER

#define ERR_CANNOTSENDTOCHAN(servername, nickname, channelName) std::string(":") + servername + " 404 " + nickname + " " + channelName + " :Cannot send to channel" + DELIMITER

#define ERR_TOOMANYCHANNELS(servername, nickname, channelName)                                                                                                                     \
	std::string(":") + servername + " 405 " + nickname + " " + channelName + " :You have joined too many channels" + DELIMITER

/**
 * example: ":karma.freenode.net 421 batman PPPP :Unknown command"
 */
#define ERR_UNKNOWNCOMMAND(servername, nickname, command) std::string(":") + servername + " 421 " + nickname + " " + command + " :Unknown command" + DELIMITER

/**
 * example: ":teepee.freenode.net 409 batman :No origin specified"
 */
#define ERR_NOORIGIN(servername, nickname) std::string(":") + servername + " 409 " + nickname + " :No origin specified" + DELIMITER

#define ERR_NOTONCHANNEL(servername, nickname, channelName) std::string(":") + servername + " 442 " + nickname + " " + channelName + " :You're not on that channel" + DELIMITER

/**
 * example: ":karma.freenode.net 451 * JOIN :You have not registered."
 */
#define ERR_NOTREGISTERED(servername, nickname, commandName) std::string(":") + servername + " 451 " + nickname + " " + commandName + " :You have not registered." + DELIMITER

/**
 * example:
 * ":karma.freenode.net 461 nickerere NICK :Not enough parameters."
 * ":karma.freenode.net 650 nickerere NICK :<newnick>"
 */
#define ERR_NEEDMOREPARAMS(servername, nickname, commandName) std::string(":") + servername + " 461 " + nickname + " " + commandName + " :Not enough parameters." + DELIMITER

#define ERR_PASSWDMISMATCH(servername, nickname) std::string(":") + servername + " 464 " + nickname + " :Password incorrect" + DELIMITER

#define ERR_CHANNELISFULL(servername, nickname, channelName) std::string(":") + servername + " 471 " + nickname + " " + channelName + " :Cannot join channel (+l)." + DELIMITER

#define ERR_UNKNOWNMODE(servername, nickname, unknownChar) std::string(":") + servername + " 472 " + nickname + " " + unknownChar + " :is unknown mode char to me" + DELIMITER

#define ERR_INVITEONLYCHAN(servername, nickname, channelName) std::string(":") + servername + " 473 " + nickname + " " + channelName + " :Cannot join channel (+i)." + DELIMITER

#define ERR_BANNEDFROMCHAN(servername, nickname, channelName) std::string(":") + servername + " 474 " + nickname + " " + channelName + " :Cannot join channel (+b)." + DELIMITER

#define ERR_BADCHANNELKEY(servername, nickname, channelName) std::string(":") + servername + " 475 " + nickname + " " + channelName + " :Cannot join channel (+k)." + DELIMITER

#define ERR_CHANOPRIVSNEEDED(servername, nickname, channelName) std::string(":") + servername + " 482 " + nickname + " " + channelName + " :You're not channel operator" + DELIMITER

#define ERR_NOOPERHOST(servername, nickname) std::string(":") + servername + " 491 " + nickname + " :No O-lines for your host" + DELIMITER

#define ERR_NOSUCHNICK(servername, nickname, nick) std::string(":") + servername + " 401 " + std::string(nickname) + " " + nick + " :No such nick/channel" + DELIMITER

#define ERR_NORECIPIENT(servername, nickname, commandName, description)                                                                                                            \
	std::string(":") + servername + " 411 " + nickname + std::string(" :No recipient given (") + commandName + ")" + DELIMITER + description + DELIMITER

#define ERR_NOTEXTTOSEND(servername, nickname) std::string(":") + servername + " 412 " + nickname + std::string(" :No text to send") + DELIMITER

#define ERR_TOOMANYTARGETS(servername, nickname, target)                                                                                                                           \
	std::string(":") + servername + " 407 " + nickname + std::string(target) + " :Duplicate recipients. No message delivered" + DELIMITER

#define ERR_NONICKNAMEGIVEN(servername, nickname) std::string(":") + servername + " 431 " + nickname + " :No nickname given" + DELIMITER

#define ERR_ERRONEUSNICKNAME(servername, nickname, nick) std::string(":") + servername + " 432 " + nickname + " " + nick + " :Erroneus nickname" + DELIMITER

#define ERR_NICKNAMEINUSE(servername, nickname, nick) std::string(":") + servername + " 433 " + nickname + " " + nick + " :Nickname is already in use" + DELIMITER

#define ERR_ALREADYREGISTRED(servername, nickname) std::string(":") + servername + " 462 " + nickname + " :You may not reregister" + DELIMITER

#define ERR_KEYSET(servername, nickname, channelname) std::string(":") + servername + " 467 " + nickname + " " + channelname + " :Channel key already set" + DELIMITER

#define ERR_UMODEUNKNOWNFLAG(servername, nickname) std::string(":") + servername + " 501 " + nickname + " :Unknown MODE flag" + DELIMITER

#define ERR_USERSDONTMATCH(servername, nickname) std::string(":") + servername + " 502 " + nickname + " :Cant change mode for other users" + DELIMITER

#define ERR_NOSUCHSERVER(servername, nickname) std::string(":") + servername + " 402 " + nickname + " " + servername + " :No such server" + DELIMITER

#define ERR_USERONCHANNEL(servername, nickname, nick, channel)                                                                                                                     \
	std::string(":") + servername + " 443 " + nickname + " " + nick + " " + channel + " :is already on channel" + DELIMITER

#endif