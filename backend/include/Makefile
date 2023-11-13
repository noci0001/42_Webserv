NAME	=	server

SRCS	=	Servers/test.cpp  \
			Servers/TestServer.cpp \
			Servers/SimpleServer.cpp \
			networking/Sockets/SimpleSocket.cpp \
			networking/Sockets/BindingSocket.cpp \
			networking/Sockets/ListeningSocket.cpp \

HEADER	=	Servers/test.hpp  \
			Servers/TestServer.hpp \
			Servers/SimpleServer.hpp \
			networking/Sockets/SimpleSocket.hpp \
			networking/Sockets/BindingSocket.hpp \
			networking/Sockets/ListeningSocket.hpp \

OBJS	=	$(SRCS:.cpp=.o)

CC		=	c++
CFLAGS	=	-Wall -Werror -Wextra -g -std=c++98
RM		=	rm -rf

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo Webserv has been successfully compiled
	echo "Executing program by calling the executable..."
	./server

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fcl