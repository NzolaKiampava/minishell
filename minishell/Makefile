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
       $(SRCS_DIR)/builtins/builtin_utils.c \
       $(SRCS_DIR)/builtins/cd.c \
       $(SRCS_DIR)/builtins/echo.c \
       $(SRCS_DIR)/builtins/env.c \
       $(SRCS_DIR)/builtins/exit.c \
       $(SRCS_DIR)/builtins/export.c \
       $(SRCS_DIR)/builtins/pwd.c \
       $(SRCS_DIR)/builtins/unset.c \
       $(SRCS_DIR)/executor/executor.c \
       $(SRCS_DIR)/executor/pipe_handler.c \
       $(SRCS_DIR)/executor/redirections.c \
       $(SRCS_DIR)/parser/parser.c \
       $(SRCS_DIR)/parser/tokenizer.c \
       $(SRCS_DIR)/parser/expander.c \
       $(SRCS_DIR)/signals/signals.c \
       $(SRCS_DIR)/utils/environment.c \
       $(SRCS_DIR)/utils/error_handler.c \
       $(SRCS_DIR)/utils/utils.c \
       $(SRCS_DIR)/utils/parser_utils.c \
       $(SRCS_DIR)/utils/token_utils.c \
       $(SRCS_DIR)/utils/token2_utils.c \
       $(SRCS_DIR)/utils/pipe_utils.c \
       $(SRCS_DIR)/utils/expand_utils.c \
       $(SRCS_DIR)/utils/export_utils.c \
       $(SRCS_DIR)/utils/redirection_utils.c \

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
