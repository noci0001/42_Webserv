NAME	=	webserv

SRCS	=	backend/src/Webserv.cpp \
			backend/src/Parsing.cpp \

HEADER	=	backend/include/Webserv.hpp

OBJS	=	$(SRCS:.cpp=.o)

CC		=	c++
CFLAGS	=	-Wall -Werror -Wextra -std=c++98
CDEBUG	=	-g3
RM		=	rm -rf

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo Webserv has been successfully compiled

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re