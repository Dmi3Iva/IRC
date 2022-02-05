#include "Bot.hpp"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main(int argc, char** argv)
{
	string hint = "./bot [host:port_network:password_network] <port> <password>\n";
	Bot* bot;
	if (argc < 2 || argc > 3) {
		cout << hint;
		return 0;
	} else if (argc == 3)
		bot = new Bot("127.0.0.1", argv[1], argv[2]);
	else {
		vector<string> args = ft_split(argv[1], ":");
		if (args.size() == 1)
			bot = new Bot("127.0.0.1", args[0], "");
		else if (args.size() != 3 || args[0].empty() || args[1].empty() || args[2].empty()) {
			cout << hint;
			return 0;
		} else {
			bot = new Bot(args[0], args[1], args[2]);
		}
	}
	bot->start();
	cout << "Bot is closing ..." << endl;
	delete bot;
	return 0;
}