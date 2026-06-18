NAME = libft.a
CC = cc
CFLAGS = -Wall -Werror -Wextra
AR = ar
ARFLAGS = rcs
HEADER = libft.h

SRC_IS__DIR = is_/
SRC_IS__FILES = ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_issign.c ft_isspace.c
SRC_IS_ = $(foreach F,$(SRC_IS__FILES),$(SRC_IS__DIR)$F)

SRC_FD_DIR = fd/
SRC_FD_FILES = ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c
SRC_FD = $(foreach F,$(SRC_FD_FILES),$(SRC_FD_DIR)$F)

SRC_LST_DIR = lst/
SRC_LST_FILES = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c \
ft_lstiter_bonus.c ft_lstmap_bonus.c ft_lstsplit.c
SRC_LST = $(foreach F,$(SRC_LST_FILES),$(SRC_LST_DIR)$F)

SRC_MEM_DIR = mem/
SRC_MEM_FILES = ft_calloc.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c \
ft_memset.c ft_bzero.c
SRC_MEM = $(foreach F,$(SRC_MEM_FILES),$(SRC_MEM_DIR)$F)


SRC_STR_ADDRESS_DIR = address/
SRC_STR_ADDRESS_FILES = ft_strchr.c ft_strnstr.c ft_strrchr.c
SRC_STR_ADDRESS = $(foreach F,$(SRC_STR_ADDRESS_FILES),$(SRC_STR_ADDRESS_DIR)$F)

SRC_STR_INT_DIR = int/
SRC_STR_INT_FILES = ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strncmp.c ft_strcmp.c str_empty.c
SRC_STR_INT = $(foreach F,$(SRC_STR_INT_FILES),$(SRC_STR_INT_DIR)$F)

SRC_STR_VOID_DIR = void/
SRC_STR_VOID_FILES = ft_striteri.c
SRC_STR_VOID = $(foreach F,$(SRC_STR_VOID_FILES),$(SRC_STR_VOID_DIR)$F)

SRC_STR_MALLOC_DIR = malloc/
SRC_STR_MALLOC_FILES = ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_strmapi.c
SRC_STR_MALLOC = $(foreach F,$(SRC_STR_MALLOC_FILES),$(SRC_STR_MALLOC_DIR)$F)

SRC_STR_DIR = str/
SRC_STR_FILES = ${SRC_STR_ADDRESS} ${SRC_STR_MALLOC} ${SRC_STR_VOID} ${SRC_STR_INT}
SRC_STR = $(foreach F,$(SRC_STR_FILES),$(SRC_STR_DIR)$F)


SRC = ft_atoi.c ft_tolower.c ft_toupper.c ft_split.c ft_itoa.c \
ptrptr.c ${SRC_STR} ${SRC_MEM} ${SRC_LST} ${SRC_IS_} ${SRC_FD}

OBJECTS = $(SRC:.c=.o)

.PHONY: all clean fclean re bonus

all: $(NAME)

$(NAME): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

#bonus: $(OBJECTS) $(BONUS_OBJECTS)
#	$(AR) $(ARFLAGS) $(NAME) $(BONUS_OBJECTS)

clean:
	rm -f $(OBJECTS) $(BONUS_OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all
