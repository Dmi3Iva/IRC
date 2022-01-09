NAME=ircserv

CC = clang++

CFLAGS = -Wall -Werror -Wextra -std=c++98

SRC = srcs/main.cpp

HEADERS = srcs/header.hpp

$(NAME): $(SRC) $(HEADERS)
	$(CC) $(SRC) $(CFLAGS) -o $(NAME)

all: $(NAME)

clean:

fclean: clean
	rm -rf $(NAME)

re: fclean all
