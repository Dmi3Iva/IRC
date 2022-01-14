#include "ACommand.hpp"

ACommand::ACommand(vector<User> *usersPtr, vector<Channel> *channelsPtr)
    : _name(""), _usersPtr(usersPtr), _channelsPtr(channelsPtr) {}

void ACommand::sendMessage(int fd, string msg) { send(fd, msg.c_str(), msg.size(), 0); }
