NAME=ircserv

CC = clang++

CFLAGS = -Wall -Werror -Wextra -std=c++98 -g

SRC = srcs/main.cpp \
	srcs/ACommand.cpp \
 	srcs/Commands/NickCommand.cpp\
 	srcs/Commands/UserCommand.cpp\
 	srcs/Commands/JoinCommand.cpp\
 	srcs/Commands/PartCommand.cpp\
	srcs/Commands/PrivateMessageCommand.cpp \
 	srcs/Commands/PingCommand.cpp\
	srcs/Commands/WhoCommand.cpp \
 	srcs/Commands/PassCommand.cpp\
 	srcs/Commands/OperCommand.cpp\
 	srcs/Commands/NamesCommand.cpp\
 	srcs/Commands/ModeCommand.cpp\
	srcs/Commands/AwayCommand.cpp\
	srcs/Commands/NoticeCommand.cpp\
	srcs/Commands/KickCommand.cpp \
	srcs/Commands/WhoisCommand.cpp \
	srcs/Commands/InviteCommand.cpp \
	srcs/Commands/QuitCommand.cpp \
	srcs/Commands/ListCommand.cpp \
	srcs/Channel.cpp \
	srcs/Context.cpp \
	srcs/Server/Server.cpp \
	srcs/Server/Socket.cpp \
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
	srcs/Commands/PrivateMessageCommand.hpp \
 	srcs/Commands/PingCommand.hpp\
	srcs/Commands/WhoCommand.hpp \
 	srcs/Commands/PassCommand.hpp\
 	srcs/Commands/OperCommand.hpp\
 	srcs/Commands/NamesCommand.hpp\
 	srcs/Commands/ModeCommand.hpp\
	srcs/Commands/AwayCommand.hpp\
	srcs/Commands/NoticeCommand.hpp\
	srcs/Commands/KickCommand.hpp \
	srcs/Commands/WhoisCommand.hpp \
	srcs/Commands/InviteCommand.hpp \
	srcs/Commands/QuitCommand.hpp \
	srcs/Commands/ListCommand.hpp \
	srcs/Channel.hpp \
	srcs/Context.hpp \
	srcs/Server/Server.hpp \
	srcs/Server/Socket.hpp \
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
