
#ifndef CONSTANTS_H
# define CONSTANTS_H

# ifdef unix
#  define IS_LINUX 1
#  define MAX_PATH 4096
# else
#  define IS_LINUX 0
#  define MAX_PATH 1024
# endif

# define GREEN "\033[0;32m"
//# define RED "\033[0;31m"
# define YELLOW "\033[0;33m"
# define CYAN "\033[0;36m"
# define GREEN_BOLD "\033[1;32m"
# define RED_BOLD "\033[1;31m"
# define YELLOW_BOLD "\033[1;33m"
# define CYAN_BOLD "\033[1;36m"
# define RESET "\033[0m"

# define CTRL_D 4
# define CTRL_C 3
# define ENTER_KEYCAP 10
# define CTRL_B 28

# define WHITE_SPACE " \t\n\v\f\r"
# define MAX_TOKENS 100

//# define EXIT_GENERAL_ERROR 2
//# define EXIT_CMD_NOT_FOUND 127
# define EXIT_CMD_INTERRUPTED 130
//# define EXIT_ERROR_FORMAT 258

#endif