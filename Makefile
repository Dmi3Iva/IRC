NAME=ircserv

CC = clang++

CFLAGS = -Wall -Werror -Wextra -std=c++98

SRC = srcs/main.cpp \
	srcs/ACommand.cpp \
	srcs/Channel.cpp \
	srcs/Context.cpp \
	srcs/Server.cpp \
	srcs/User.cpp \
	srcs/utils.cpp

OBJ=$(SRC:.cpp=.o)

HEADERS = srcs/constants.hpp \
 	srcs/ACommand.hpp \
	srcs/Channel.hpp \
	srcs/Context.hpp \
	srcs/Server.hpp \
	srcs/User.hpp \
	srcs/utils.hpp

srcs/%.o: srcs/%.cpp $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ) $(HEADERS)
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
