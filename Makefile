# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/11 14:57:50 by nkiampav          #+#    #+#              #
#    Updated: 2024/11/11 16:15:46 by nkiampav         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror -I$(LIBFT_DIR)

SRCS = minishell.c \
       input.c \
       parser.c \
       signals.c \
       utils.c \
       builtins.c \

# Libft directory and library
LIBFT_DIR = ./libft
LIBFT = -L$(LIBFT_DIR) -lft

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT_DIR)/libft.a:
	@echo "Making Libft...."
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -sC $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -sC $(LIBFT_DIR) fclean

re: flcean all

.PHONY: all clean fclean re
