NAME=ircserv

CC = clang++

CFLAGS = -Wall -Werror -Wextra -std=c++98 -g

SRC = srcs/main.cpp \
	srcs/ACommand.cpp \
 	srcs/Commands/NickCommand.cpp\
 	srcs/Commands/UserCommand.cpp\
 	srcs/Commands/JoinCommand.cpp\
 	srcs/Commands/PartCommand.cpp\
 	srcs/Commands/PrivmsgCommand.cpp\
	srcs/Channel.cpp \
	srcs/Context.cpp \
	srcs/Server.cpp \
	srcs/User.cpp \
	srcs/utils.cpp

OBJ=$(SRC:.cpp=.o)

HEADERS = srcs/constants.hpp \
	srcs/responses.hpp \
 	srcs/ACommand.hpp \
 	srcs/Commands/NickCommand.hpp\
 	srcs/Commands/UserCommand.hpp\
 	srcs/Commands/JoinCommand.hpp\
 	srcs/Commands/PartCommand.hpp\
 	srcs/Commands/PrivmsgCommand.hpp\
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
