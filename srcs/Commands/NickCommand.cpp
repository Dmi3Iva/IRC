#include "NickCommand.hpp"

NickCommand::NickCommand(string serverName, userVector *usersPtr, channelMap *channelsPtr)
    : ACommand(serverName, usersPtr, channelsPtr) {
    _name = "NICK";
    _description = "NICK <nickname> [ <hopcount> ]";
}

NickCommand::~NickCommand() {}

void NickCommand::execute(User *user, string cmd) {
    cout << "Execute command " << cmd << endl;
    string nick = _getNickname(cmd);
    if (!_validateNick(nick)) {
        return;
    }
    user->setNickname(cmd);
    if (!user->getNickname().empty() && !user->getRealname().empty() && !user->getUsername().empty()) {
        user->setIsRegistered(true);
        string msg =                                        //
            RPL_MOTDSTART(_serverName, user->getNickname()) //
            RPL_MOTD(_serverName, user->getNickname())      //
            RPL_ENDOFMOTD(_serverName, user->getNickname());
        sendMessage(user->getFD(), msg);
    }
}

string NickCommand::_getNickname(string cmd) {
    eraseSpacesInFront(cmd);

    int i = 0;
    while (!findCharInSring(cmd[i], " \n\r\0\t") && cmd[i]) {
        i++;
    }
    string nick = cmd.substr(0, i);
    return (nick);
}

bool NickCommand::_validateNick(string nick) {
    if (nick.size() >= 10) {
        return (false);
    }
    if (isdigit(nick[0]))
        return (false);
    for (int i = 0; nick[i] != '\0'; i++) {
        if (!findCharInSring(nick[i], ALLOWED_NICK_SECIAL_CHARACTERS) && !isalpha(nick[i]) && !isdigit(nick[i])) {
            cout << "UnAllowed character in NIckname " << nick[i] << endl;
            return (false);
        }
    }
    for (userVector::iterator it = _usersPtr->begin(); it != _usersPtr->end(); it++) {
        if ((*it)->getNickname() == nick) {
            cout << "NICKName - " << nick << " already exitst" << endl;
            return (false);
        }
    }
    return (true);
}
