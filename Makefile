NAME				=	webserv

CC					=	c++

CPPFLAGS			=	-Wall -Wextra -Werror -std=c++98 -g -O3 #-fsanitize=address

HPP_DIR				=	./backend/include

OBJECT_DIR			=	./backend/objs

CPP_DIR				=	./backend/src

SRC					=	$(shell find $(CPP_DIR) -name '*.cpp')

HEADERS				=	-I$(HPP_DIR)

OBJ					=	$(patsubst $(CPP_DIR)/%.cpp, $(OBJECT_DIR)/%.o, $(SRC))

BASE				=	\033[38;5;
RED					=	\033[38;5;196m
RED_B				=	\033[48;5;160m
GREEN				=	\033[38;5;40m
GREEN_B				=	\033[48;5;40m
COMPILATION			=	\033[48;5;40m[COMPILATION]\033[0m $(GREEN)
BLUE				=	\033[38;5;123m
BLUE_B				=	\033[48;5;39m
INFO				=	\033[48;5;39m[INFORMATION]\033[0m $(BLUE)
TESTING_B			=	\033[48;5;202m[ TESTING MODE IS NOW ACTIVATED ]\033[0m
TESTING_T			=	\033[38;5;202m
RESET				=	\033[0m
BOLD				=	\033[1m
ITALIC				=	\033[3m
UNDERLINE			=	\033[4m
STRIKE				=	\033[9m

CURRENT_PROGRESS	=	0
TOTAL_PROGRESS		=	$(words $(OBJ))

PROGRESS_WIDTH		=	33
OS					=	Mac
PROGRESS_DONE_TILE	=	🟩
PROGRESS_TODO_TILE	=	⬜️

all:				ascii-art $(NAME)

$(NAME):			$(OBJ)
					@$(CC) $(CPPFLAGS) $(HEADERS) $(OBJ) -o $(NAME)
					@echo "$(RESET)$(INFO)🖥️ $@ Webserv ready to use$(RESET)"

$(OBJECT_DIR)/%.o:	$(CPP_DIR)/%.cpp
					@mkdir -p $(@D)
					@$(CC) $(CPPFLAGS) $(HEADERS) -c $^ -o $@
					$(eval CURRENT_PROGRESS=$(shell echo $$(($(CURRENT_PROGRESS)+1))))
					$(eval PERCENTAGE=$(shell echo $$(($(CURRENT_PROGRESS)*100/$(TOTAL_PROGRESS)))))
					@if [ $(CURRENT_PROGRESS) -eq $(TOTAL_PROGRESS) ]; then \
						printf "\033[2A\r\033[K$(GREEN_B)[COMPILATION]$(RESET) \033[38;5;40m$(PERCENTAGE)%% [$$(printf '%*s' $$(($(CURRENT_PROGRESS)*$(PROGRESS_WIDTH)/$(TOTAL_PROGRESS))) | tr ' ' '$(PROGRESS_DONE_TILE)')$$(printf '%*s' $$(($(PROGRESS_WIDTH)-($(CURRENT_PROGRESS)*$(PROGRESS_WIDTH)/$(TOTAL_PROGRESS)))) | tr ' ' '$(PROGRESS_TODO_TILE)')] $<\033[0m ✅$(RESET)\n"; \
					else \
						printf "\033[2A\r\033[K$(GREEN_B)[COMPILATION]$(RESET) \033[38;5;51m$(PERCENTAGE)%% [$$(printf '%*s' $$(($(CURRENT_PROGRESS)*$(PROGRESS_WIDTH)/$(TOTAL_PROGRESS))) | tr ' ' '$(PROGRESS_DONE_TILE)')$$(printf '%*s' $$(($(PROGRESS_WIDTH)-($(CURRENT_PROGRESS)*$(PROGRESS_WIDTH)/$(TOTAL_PROGRESS)))) | tr ' ' '$(PROGRESS_TODO_TILE)')] $<$(RESET)\n\n"; \
					fi
					@sleep 0.01

clean:
					@for obj in $(OBJ); do\
						rm -f $$obj; \
						printf "$(RED_B)[Deleted]$(RESET) $(RED)$$obj$(RESET)\n"; \
					done

fclean:				clean
					@rm -f $(NAME);
					@rm -rf $(OBJECT_DIR);
					@printf "$(RED_B)[Deleted]$(RESET) $(RED)$(NAME)$(RESET)\n";

re:					fclean all

ascii-art:
					@clear
					@echo "$(BASE)231m"
					@echo "$(BASE)231m  _  _  ____   __        __   _      __ _ _      "
					@echo "$(BASE)231m | || ||___ \  \ \      / /__| |__  / _| (_)_  __"
					@echo "$(BASE)195m | || |_ __) |  \ \ /\ / / _ \ '_ \| |_| | \ \/ /"
					@echo "$(BASE)195m |__   _/ __/    \ V  V /  __/ |_) |  _| | |>  < "
					@echo "$(BASE)195m    |_||_____|    \_/\_/ \___|_.__/|_| |_|_/_/\_\\"
					@echo "$(BASE)195m                                                 "
					@echo "$(BASE)159m [$(ITALIC)This webserv brought to you by 🤯 @ydimitro & @snocita & @tstrassb$(RESET)$(BASE)159m]$(RESET)"; echo; echo; echo;
					@read -p "Press [Enter] to continue"

.PHONY:				all clean fclean re