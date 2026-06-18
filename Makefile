NAME = minishell
TEST = test
CC = gcc
CFLAGS = -Wall -Werror -Wextra -g
HEADER = include/ libs/libft libs/ft_printf
ARC_PRINTF = libs/ft_printf/libftprintf.a -lreadline
ARC_LIBFT = libs/libft/libft.a
ARC = $(ARC_PRINTF) $(ARC_LIBFT)
INCLUDE = $(foreach H,$(HEADER),-I $H)

SRC_AST_DIR = ast_build/
SRC_AST_FILES = redirs_utils.c ast_build.c
SRC_AST = $(foreach F,$(SRC_AST_FILES),$(SRC_AST_DIR)$F)

SRC_TOKENS_DIR = tokens/
SRC_TOKENS_FILES = tokens_get.c tokens_syntax_error.c tokens_values.c
SRC_TOKENS = $(foreach F,$(SRC_TOKENS_FILES),$(SRC_TOKENS_DIR)$F)

SRC_CLEANUP_DIR = cleanup/
SRC_CLEANUP_FILES = cleanup.c cleanup_tokens.c cleanup_ast.c
SRC_CLEANUP = $(foreach F,$(SRC_CLEANUP_FILES),$(SRC_CLEANUP_DIR)$F)

SRC_PARSE_DIR = parsing/
SRC_PARSE_FILES = parse.c $(SRC_AST) $(SRC_TOKENS)
SRC_PARSE = $(foreach F,$(SRC_PARSE_FILES),$(SRC_PARSE_DIR)$F)

SRC_DIR = src/
SRC_FILES = main.c get_line.c setup.c signals.c test.c utils.c resolve_hd.c $(SRC_PARSE) $(SRC_CLEANUP) 
SRC = $(foreach F,$(SRC_FILES),$(SRC_DIR)$F)

OBJ = $(SRC:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ) $(ARC_LIBFT) $(ARC_PRINTF)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) $(ARC) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean :
	rm -f $(OBJ)
	make clean -C libs/ft_printf
	make clean -C libs/libft

fclean : clean
	make fclean -C libs/ft_printf
	make fclean -C libs/libft
	rm -f $(NAME)

re : fclean all

$(ARC_PRINTF) :
	make -C libs/ft_printf/

$(ARC_LIBFT) :
	make -C libs/libft/

.PHONY = all re clean fclean
