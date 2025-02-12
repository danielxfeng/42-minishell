# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/08 15:25:33 by Xifeng            #+#    #+#              #
#    Updated: 2025/02/08 19:12:40 by Xifeng           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# NAME := minishell
# CC := cc
# FLAG := -Wall -Wextra -Werror

# LIBFT := $(LIBFT_DIR)/libft.a
# LIBFT_PATH :=  ./lib/libft
# SRC_PATH := src
# HEADERS := -I ./inc -I $(LIBFT_DIR)/inc
# LINK := -L$(LIBFT_DIR) -lft

# SRC := $(shell find $(SRC_PATH) -type f -name "*.c")

NAME := minishell
CC := cc
RM := rm -rf
FLAG := -Wall -Wextra -Werror

LIBFT_DIR := ./src/libs/libft
LIBFT := $(LIBFT_DIR)/libft.a
HEADERS := -I ./src -I $(LIBFT_DIR)/libft.h #chage to /inc
LINK := -L$(LIBFT_DIR) -lft -lncurses

SRC_DIR := src
#SRC := $(shell ls src/*.c)
SRC := $(shell find $(SRC_DIR) -type f -name "*.c") #| grep -v -E 'ast|executor')
#SRC += src/main.c

OBJ_DIR := obj
OBJ := $(SRC:src%.c=obj%.o)

all: $(NAME)

$(LIBFT): $(LIBFT_DIR)
	@echo "Building libft..."
	make -C $(LIBFT_DIR);

# $(LIBFT_DIR):
# 	git clone https://github.com/IbnBaqqi/libft.git $@;

$(OBJ_DIR):
	mkdir obj

$(NAME):$(LIBFT) $(OBJ_DIR) $(OBJ)
	$(CC) $(OBJ) $(LINK) $(HEADERS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(FLAG) $< -o $@ $(HEADERS)

clean:
	$(RM) $(NAME)
	$(RM) $(LIBFT_DIR)/*.o
	$(RM) $(LIBFT_DIR)/libft.a

fclean: clean
	@rm -f $(NAME)
	$(RM) $(OBJ_DIR)
#	$(RM) $(LIBFT_DIR)/libft.a
#	$(RM) $(LIBFT_DIR)

re: fclean all