NAME = libftprintf.a

LIBFT = ../libft/libft.a
LIBFT_DIR = ../libft
LIBFT_HEADER = ../libft/libft.h

CC = cc
CFLAGS = -Wall -Werror -Wextra -I./ -I../libft

HEADERS = ft_printf.h $(LIBFT_HEADER)

AR = ar
ARFLAGS = rcs

SRCS_DIR = ./
SRCS = ft_printf.c ft_printf_utils.c printdec.c printhex.c printptr.c printuns.c

OBJECTS = ${addprefix ${SRCS_DIR}, ${SRCS:.c=.o}}

all : $(LIBFT) ${NAME}

${NAME} : ${OBJECTS}
	${AR} ${ARFLAGS} $@ $^

${LIBFT} :
	make -C $(LIBFT_DIR)

${SRCS_DIR}%.o : ${SRCS_DIR}%.c $(HEADERS)
	${CC} ${CFLAGS} -c $< -o $@

clean :
	rm -rf ${OBJECTS}

fclean : clean
	rm -rf ${NAME}

re : fclean all

.PHONY = all re clean fclean
