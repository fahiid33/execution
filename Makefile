
# LDFLAGS = -L/home/fahd/.brew/Cellar/readline/8.1.2/lib
# CPPFLAGS = -I/home/fahd/.brew/Cellar/readline/8.1.2/include

NAME = minishell
NAME_BONUS = 

SRC = minishell.c lexer.c tokenizing.c utils.c parse_utils.c builtins.c split.c\

		
SRC_BONUS = 


OBJ =  minishell.o lexer.o tokenizing.o utils.o parse_utils.o builtins.o split.o\

OBJ_BONUS = 


all : $(NAME)

$(NAME):
	
	@gcc  -g $(SRC) -lreadline  -o $(NAME)
	@gcc  -g $(SRC) -lreadline -o $(NAME)
	@echo "👍👍👍"

$(NAME_BONUS) : 
	@gcc -Wall -Wextra -Werror -g $(SRC_BONUS) -o $(NAME_BONUS)
	@echo "🎁🎁🎁"

bonus: $(NAME_BONUS)

clean :
 
	@rm -f $(OBJ) $(OBJ_BONUS)
	@echo "🚮🚮🚮"

fclean : clean
	@rm -f $(NAME) $(NAME_BONUS)
	@echo "🗑️ 🗑️ 🗑️"

re : fclean all