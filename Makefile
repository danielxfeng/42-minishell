NAME := minishell
CC := cc
RM := rm -rf
FLAG := -Wall -Wextra -Werror

LIBFT_DIR := ./src/libs/libft
LIBFT := $(LIBFT_DIR)/libft.a
HEADERS := -I ./src -I $(LIBFT_DIR)
LINK := -L$(LIBFT_DIR) -lft -lncurses -lreadline

SRC_DIR := src
SRC := \
    ./src/utils/executor_utils.c \
    ./src/utils/parser_utils.c \
    ./src/utils/signals_supp.c \
    ./src/utils/signals.c \
    ./src/adaptor/build_tree.c \
    ./src/ast/ast_printer_supp.c \
    ./src/ast/ast_printer.c \
    ./src/ast/ast_close_supp.c \
    ./src/ast/ast_factory.c \
    ./src/ast/ast_close.c \
    ./src/builtin/builtin_env_funcs.c \
    ./src/builtin/builtin_env_funcs_supp.c \
    ./src/builtin/builtin_supp.c \
    ./src/builtin/builtin_funcs.c \
    ./src/builtin/builtin_directory_funcs.c \
    ./src/builtin/builtin.c \
    ./src/executor/pipe_handler.c \
    ./src/executor/pre_travelsal_handler.c \
    ./src/executor/red_handler.c \
    ./src/executor/cmd_handler_supp2.c \
    ./src/executor/cmd_handler_supp.c \
    ./src/executor/expander_in_here_doc.c \
    ./src/executor/cmd_handler.c \
    ./src/executor/supp.c \
    ./src/executor/pre_travelsal_handler_supp.c \
    ./src/executor/pre_travelsal_hander_supp2.c \
    ./src/env/env_factory.c \
    ./src/env/env_methods.c \
    ./src/env/env_supp.c \
    ./src/exit_handler/parser_exit_handler.c \
    ./src/exit_handler/shell_exit_handler.c \
    ./src/exit_handler/executor_exit_handler.c \
    ./src/exit_handler/builtin_exit_handler.c \
    ./src/parser/parser_factory.c \
    ./src/parser/tokens_methods.c \
    ./src/parser/parser_handler.c \
    ./src/parser/expander_handler.c \
    ./src/parser/parser.c \
    ./src/parser/parser_supp.c \
    ./src/parser/quote_handler.c \
    ./src/shell/shell.c \
    ./src/main.c

OBJ_DIR := obj
OBJ := $(SRC:./src/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(LIBFT): 
	@echo "Building libft..."
	make -C $(LIBFT_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(OBJ) $(LINK) $(HEADERS) -o $@

$(OBJ_DIR)/%.o: ./src/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(FLAG) $< -o $@ $(HEADERS)

clean:
	$(RM) $(OBJ_DIR)/*.o
	$(RM) $(LIBFT_DIR)/*.o
	$(RM) $(LIBFT_DIR)/libft.a

fclean: clean
	@rm -f $(NAME)

re: fclean all
