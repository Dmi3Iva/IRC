#include "Server.hpp"

// TODO:: parse args
/**
 * Parse args, if they okay execute server
 * @return
 */
int main(void)
{
	Server* app = Server::getInstance();
	app->start();
	return (0);
}
