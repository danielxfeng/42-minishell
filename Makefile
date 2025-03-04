NAME := minishell
CC := cc
RM := rm -rf
FLAG := -Wall -Wextra -Werror

LIBFT_DIR := ./src/libs/libft
LIBFT := $(LIBFT_DIR)/libft.a
HEADERS := -I ./src -I $(LIBFT_DIR) #-I/opt/homebrew/opt/readline/include
LINK := -L$(LIBFT_DIR) -lft -lncurses -lreadline #-L/opt/homebrew/opt/readline/lib

SRC_DIR := src
SRC := $(shell find $(SRC_DIR) -type f -name "*.c")

OBJ_DIR := obj
OBJ := $(SRC:src%.c=obj%.o)

all: $(NAME)

$(LIBFT):
	@echo "Building libft..."
	make -C $(LIBFT_DIR);

$(OBJ_DIR):
	mkdir obj

$(NAME):$(LIBFT) $(OBJ_DIR) $(OBJ)
	$(CC) $(OBJ) $(LINK) $(HEADERS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(FLAG) $< -o $@ $(HEADERS)

clean:
	$(RM) $(LIBFT_DIR)/*.o
	$(RM) $(LIBFT_DIR)/libft.a

fclean: clean
	@rm -f $(NAME)
	$(RM) $(OBJ_DIR)

re: fclean all