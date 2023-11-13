NAME	=	webserv

<<<<<<< HEAD
SRCS	=	backend/src/Webserv.cpp \
			backend/src/Parsing.cpp \
			backend/src/Configfile.cpp \
			backend/src/utils/ParsingUtils.cpp \
			backend/src/utils/Utils.cpp \
			backend/src/Location.cpp \
			backend/src/ServerConfig.cpp \
			backend/src/MimeType.cpp \
			backend/src/Response.cpp \
=======
SRCS	=	backend/src/CgiHandler.cpp \
>>>>>>> de50599ac616b265271b3ce6b0c1ed598d2728c6
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
CFLAGS	=	-Wall -Werror -Wextra -std=c++98
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