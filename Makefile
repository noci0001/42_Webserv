NAME	=	webserv

SRCS	=	backend/src/Webserv.cpp \
			backend/src/Parsing.cpp \
			backend/src/Configfile.cpp \
			backend/src/utils/ParsingUtils.cpp \
			backend/src/utils/Utils.cpp \
			backend/src/Location.cpp \
			backend/src/ServerConfig.cpp \

HEADER	=	backend/include/Webserv.hpp \
			backend/include/Locations.hpp \
			backend/include/ServerConfig.hpp \

OBJS	=	$(SRCS:.cpp=.o)

CC		=	c++
CFLAGS	=	-Wall -Werror -Wextra -g3 -std=c++98
RM		=	rm -rf

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS) $(MEMCHECK)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo Webserv has been successfully compiled
	echo "Executing program by calling the executable..."
	./webserv

$(MEMCHECK):
	@echo Checking for leaks...
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) webserv.conf
	

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re