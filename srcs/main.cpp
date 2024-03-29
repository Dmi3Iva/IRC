#include "Server/Server.hpp"

/**
 * Parse args, if they are okay to execute server
 * @return
 */
int main(int argc, char** argv)
{
	Server* app;
	string hint = "./ircserv [host:port_network:password_network] <port> <password>\n";

	if (argc < 2 || argc > 3) {
		cout << hint;
		return 0;
	} else if (argc == 3)
		app = new Server("127.0.0.1", argv[1], argv[2]);
	else {
		vector<string> args = ft_split(argv[1], ":");

		if (args.size() == 1)
			app = new Server("127.0.0.1", args[0], "");
		else if (args.size() != 3 || args[0].empty() || args[1].empty() || args[2].empty()) {
			cout << hint;
			return (0);
		} else {
			app = new Server(args[0], args[1], args[2]);
		}
	}

	app->start();
	delete app;
	return (0);
}
