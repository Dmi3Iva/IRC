//
// Created by Shandy Mephesto on 1/10/22.
//

#ifndef IRC_IRCCHANNEL_HPP
#define IRC_IRCCHANNEL_HPP

#include <string>
#define IRC_CHANNEL_NAME_LENGTH 200

using std::string;

class IrcChannel {
public:
  IrcChannel() {}

private:
  string name;
};

#endif // IRC_IRCCHANNEL_HPP
