NAME	=	webserv

SRCS	=	backend/src/CgiHandler.cpp \
			backend/src/ClientSide.cpp \
			backend/src/Configfile.cpp \
			backend/src/ConsoleLog.cpp \
			backend/src/HttpRequest.cpp \
			backend/src/Location.cpp \
			backend/src/main.cpp \
			backend/src/MimeType.cpp \
			backend/src/ParsingUtils.cpp \
			backend/src/Response.cpp \
			backend/src/ServerConfig.cpp \
			backend/src/ServerControler.cpp \
			backend/src/utils/Utils.cpp \

HEADER	=	backend/include/CgiHandler.hpp \
			backend/include/ClientSide.hpp \
			backend/include/Configfile.hpp \
			backend/include/ConsoleLog.hpp \
			backend/include/HttpRequest.hpp \
			backend/include/Location.hpp \
			backend/include/MimeType.hpp \
			backend/include/ParsingUtils.hpp \
			backend/include/Response.hpp \
			backend/include/ServerConfig.hpp \
			backend/include/ServerControler.hpp \
			backend/include/StatusCode.hpp \
			backend/include/Webserv.hpp

OBJS	=	$(SRCS:.cpp=.o)

CC		=	g++
CFLAGS	=	-Wall -Werror -Wextra -g -std=c++98
RM		=	rm -rf

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS) $(MEMCHECK)
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