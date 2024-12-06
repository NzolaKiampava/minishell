NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

# Directories
SRCS_DIR = src
INCLUDES_DIR = includes
LIBFT_DIR = libft
OBJ_DIR = obj

# Source files
SRCS = $(SRCS_DIR)/main.c \
       $(SRCS_DIR)/executor/pipe_handler.c \
       $(SRCS_DIR)/executor/redirections.c \
       $(SRCS_DIR)/parser/parser.c \
       $(SRCS_DIR)/parser/tokenizer.c \
       $(SRCS_DIR)/signals/signals.c \
       $(SRCS_DIR)/utils/environment.c \
       $(SRCS_DIR)/utils/error_handler.c

# Object files
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJ_DIR)/%.o)

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a
LIBS = -L$(LIBFT_DIR) -lft -lreadline

# Include paths
INCLUDES = -I$(INCLUDES_DIR) -I$(LIBFT_DIR)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
