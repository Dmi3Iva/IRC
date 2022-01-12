NAME=ircserv

CC = clang++

CFLAGS = -Wall -Werror -Wextra -std=c++98 -g

SRC = srcs/main.cpp srcs/Server.cpp srcs/User.cpp

HEADERS = srcs/header.hpp srcs/Server.hpp srcs/User.hpp

$(NAME): $(SRC) $(HEADERS)
	$(CC) $(SRC) $(CFLAGS) -o $(NAME)

all: $(NAME)

clean:

fclean: clean
	rm -rf $(NAME)

re: fclean all
