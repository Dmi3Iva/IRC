#include <iostream>
#include <string>

#include "Bot.hpp"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char** argv)
{
	if (argc != 1 && argc != 4) {
		cout << "Please use next :" << endl << "./bot [<address> <port> <password>]" << endl;
		return 0;
	}
	Bot b(argc == 1 ? "127.0.0.1" : argv[1], argc == 1 ? "6667" : argv[2], argc == 1 ? "1" : argv[3]);
	b.start();
	return 0;
}