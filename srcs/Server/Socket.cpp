//
// Created by Twycross Redfort on 1/17/22.
//

#include "Socket.hpp"

Socket::Socket() {}

Socket::Socket(int domain, int type, int protocol, int port, in_addr_t ip, int backlog) {
  _address.sin_family = domain;
  _address.sin_port = htons(port);
  _address.sin_addr.s_addr = ip;
  _sockfd = socket(domain, type, protocol);
  testConnection(_sockfd);
  _backlog = backlog;
}

Socket::~Socket() {
  close(_sockfd);
}

const sockaddr_in &Socket::getAddress() const { return _address; }

int Socket::getSockfd() const { return _sockfd; }

int Socket::getBacklog() const { return _backlog; }

void Socket::allowReuseAddress() {
  int on = 1;
  int result = setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  testConnection(result);
}

void Socket::setNonblockMode() {
  int result = fcntl(_sockfd, F_SETFL, O_NONBLOCK);
  testConnection(result);
}

void Socket::bindSocket() {
  int result = bind(_sockfd, reinterpret_cast<const struct sockaddr *>(&_address), sizeof(_address));
  testConnection(result);
}

void Socket::startListening() {
  int result = listen(_sockfd, _backlog);
  testConnection(result);
}

void Socket::testConnection(int itemToTest) {
  if (itemToTest < 0) {
    perror("Failed to connect...");
    exit(EXIT_FAILURE);
  }
}
