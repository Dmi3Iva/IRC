NAME=ircserv

CC = clang++

CFLAGS = -Wall -Werror -Wextra -std=c++98 -g

SRC = srcs/main.cpp srcs/Application.cpp srcs/User.cpp

HEADERS = srcs/header.hpp srcs/Application.hpp srcs/User.hpp

$(NAME): $(SRC) $(HEADERS)
	$(CC) $(SRC) $(CFLAGS) -o $(NAME)

all: $(NAME)

clean:

fclean: clean
	rm -rf $(NAME)

re: fclean all
