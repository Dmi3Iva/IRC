//
// Created by Twycross Redfort on 1/17/22.
//

#include "Socket.hpp"

Socket::Socket() { }

Socket::Socket(int domain, int type, int protocol)
	: _backlog(0)
{
	_sockfd = socket(domain, type, protocol);
	testConnection(_sockfd);
}

Socket::~Socket() { close(_sockfd); }

int Socket::getSockfd() const { return _sockfd; }

int Socket::getBacklog() const { return _backlog; }

void Socket::setAddressReuseMode()
{
	int on = 1;
	int result = setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	testConnection(result);
}

void Socket::setNonblockMode()
{
	int result = fcntl(_sockfd, F_SETFL, O_NONBLOCK);
	testConnection(result);
}

void Socket::bindToAddress(struct sockaddr* address)
{
	int result = bind(_sockfd, address, sizeof(sockaddr));
	testConnection(result);
}

void Socket::startListening(int backlog)
{
	_backlog = backlog;
	int result = listen(_sockfd, _backlog);
	testConnection(result);
}

void Socket::testConnection(int itemToTest)
{
	if (itemToTest < 0) {
		perror("Failed to connect...");
		exit(EXIT_FAILURE);
	}
}
