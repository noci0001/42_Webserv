NAME	=	webserv

SRCS	=	backend/src/Webserv.cpp \
			backend/src/Configfile.cpp \
			backend/src/ParsingUtils.cpp \
			backend/src/utils/Utils.cpp \
			backend/src/Location.cpp \
			backend/src/ServerConfig.cpp \
			backend/src/MimeType.cpp \
			backend/src/Response.cpp \
			backend/src/ClientSide.cpp \
			backend/src/ConsoleLog.cpp \
			backend/src/HttpRequest.cpp \
			backend/src/ServerControler.cpp \
			backend/src/CgiHandler.cpp \

HEADER	=	backend/include/Webserv.hpp \
			backend/include/Locations.hpp \
			backend/include/ServerConfig.hpp \

OBJS	=	$(SRCS:.cpp=.o)

CC		=	c++
CFLAGS	=	-Wall -Werror -Wextra -g3 -std=c++98
RM		=	rm -rf

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo Webserv has been successfully compiled

$(MEMCHECK):
	@echo Checking for leaks...
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) webserv.conf
	

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re