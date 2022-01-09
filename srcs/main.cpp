#include <cerrno>
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <cstring>
#include <iostream>
#include <netinet/in.h> // For sockaddr_in
#include <sys/socket.h> // For socket functions
#include <unistd.h>

// Source: https://ncona.com/2019/04/building-a-simple-server-with-cpp/
int main(void) {
  // Create a socket (IPv4, TCP)
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    std::cout << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << "Socket created succesfully!" << std::endl;

  // Listen to port 9999 on any address
  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(
      9999); // htons is necessary to convert a number to network byte order
  // TODO:: add cast
  if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
    std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << "Binded to port succesfully!" << std::endl;

  // Start listening. Hold at most 10 connections in the queue
  if (listen(sockfd, 10) < 0) {
    std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Grab a connection from the queue
  size_t addrlen = sizeof(sockaddr);
  int connection =
      accept(sockfd, (struct sockaddr *)&sockaddr, (socklen_t *)&addrlen);
  if (connection < 0) {
    std::cout << "Failed to grab connection. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Read from the connection
  char buffer[100];
  ssize_t bytesRead;
  while ((bytesRead = recv(connection, buffer, 100, MSG_CMSG_CLOEXEC))) {
    if (bytesRead == -1) {
      std::cout << "Something went wrong. errro: " << errno << std::endl;
      // TODO:: should I close connection before exit?
      close(connection);
      close(sockfd);
      exit(EXIT_FAILURE);
    }
    buffer[bytesRead] = '\0';
    std::cout << "The message was: " << buffer;
    if (bytesRead == 1 && buffer[0] == '#') {
      std::cout << "end signal from the client" << std::endl;
      close(connection);
      close(sockfd);
      exit(EXIT_FAILURE);
    }
    // Send a message to the connection
    std::string response(buffer);
    std::cout << " response size: " << response.size() << std::endl;
    if (static_cast<size_t>(bytesRead) != response.size())
      std::cout << "ALERT! SOME TRASH IN THE MEM" << std::endl;
    send(connection, response.c_str(), bytesRead, 0);
  }

  // Close the connections
  close(connection);
  close(sockfd);
}