/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 21:04:45 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/16 09:36:59 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "mini_env.h"
# include "utils.h"
# include <stdbool.h>
# include <stdio.h>
# include <sys/types.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# define DEBUG_FLAG 0
# define INVALID_ERR_NO -1
# define STD_IN 0
# define STD_OUT 1
# define LEFT 0
# define RIGHT 1
# define EXIT_OK 0
# define EXIT_FAIL 1
# define EXIT_INVALID_OPTION 2
# define EXIT_EXEC_ERR 126
# define EXIT_CMD_ERR 127

typedef struct s_ast_node	t_ast_node;

// Enum: type of AST nodes.
typedef enum e_node_type
{
	PIPE,
	RED,
	CMD
}							t_node_type;

// Represents the Abstract Syntax Tree as well as
// the main data structure of executor.
//
// `root` the root node of AST tree.
// `tokens` the array of tokens.
// `tk_size: the size of tokens.
// `fd_in`: the std_in before executing the AST execution.
// `fd_out` the std_out before executing the AST execution.
// `env`: the pointer to env.
//
// The std_in/out may be redirected in sub-process, so we need the
// original std_in/out, in case we need read/print to terminal.
typedef struct s_ast
{
	t_ast_node				*root;
	char					**tokens;
	int						tk_size;
	int						fd_in;
	int						fd_out;
	t_env					*env;
	// leave ur stuff here @abdul
	// history
	// maybe others
}							t_ast;

// Represents a node of AST.
//
// When execution, we apply a Pre-order traverlsal first
// to handle the file opening. (pre_handler)
// I failed to avoid this after several tries.
//
// Then we apply a mixed Pre-Post-order traversal to perform
// the execution.
//
// `prop`: the specific properties depends on `t_node_type`.
// `node_pre_handler`: the pointer to the function
// which performs the preparation jobs like opening the file.
// `node_handler`: the pointer to the function
// which performs as a executor.
// `node_closer`: the pointer to close the node.
// `node_printer`: the pointer to the printer function.
// `left` `right`: left/right child-node.
typedef struct s_ast_node
{
	t_node_type				type;
	void					*prop;
	int						(*node_pre_handler)(t_ast *ast, t_ast_node *node);
	int						(*node_handler)(t_ast *ast, t_ast_node *node);
	void					(*node_closer)(t_ast_node *ast_node);
	void					(*node_printer)(t_ast *ast, t_ast_node *n,
							t_ast_node *c, int l);
	t_ast_node				*left;
	t_ast_node				*right;
}							t_ast_node;

// Represents properties of CMD.
//
// `start` the index of start token for this command.
// `size` how many tokens are for this command?
// `pid` sub-process for running the `cmd`.
// `argv` the argv to be executed.
// `full_cmd` the cmd with path.
typedef struct s_cmd_prop
{
	int						start;
	int						size;
	pid_t					pid;
	char					**argv;
	char					*full_cmd;
}							t_cmd_prop;

// Represents properties of RED.
//
// `idx` the index of token points to the file name.
// `fd`: the file descriptor of a file.
// `is_in`: `true` for `<` or `<<`.
// `is_single`: `true` for `<` or '>'.
// `is_skip`: when there is several `red`s with same direction,
// only the right most `red` will set.
// has been set.
// `status`: saved status code on file opening error.
typedef struct s_red_prop
{
	int						idx;
	int						fd;
	bool					is_in;
	bool					is_single;
	bool					is_skip;
	int						status;
}							t_red_prop;

// Represents properties of PIPE.
//
// `pids`: pid of sub-process.
typedef struct s_pipe_prop
{
	pid_t					pids[2];
}							t_pipe_prop;

// AST
// The constructors of AST.

t_ast						*create_ast(char **tokens, int tk_size, t_env *env);
t_ast_node					*create_pipe_node(t_ast *ast);
t_ast_node					*create_cmd_node(t_ast *ast, int start, int size);
t_ast_node					*create_red_node(t_ast *ast, int idx, bool is_in,
								bool is_single);

// The destructors of AST.

int							close_ast(t_ast **ast);
void						close_pipe_node(t_ast_node *node);
void						close_cmd_node(t_ast_node *node);
void						close_red_node(t_ast_node *node);

// The pre_travelsal_handlers.

int							pre_pipe_handler(t_ast *ast, t_ast_node *ast_node);
int							pre_cmd_handler(t_ast *ast, t_ast_node *ast_node);
int							pre_red_handler(t_ast *ast, t_ast_node *ast_node);

// The execution handlers.

int							pipe_handler(t_ast *ast, t_ast_node *ast_node);
int							cmd_handler(t_ast *ast, t_ast_node *ast_node);
int							red_handler(t_ast *ast, t_ast_node *ast_node);

// Printer of AST.

void						debug_print_ast(t_ast *ast, t_ast_node *curr,
								char *msg);
void						print_pipe_node(t_ast *ast, t_ast_node *node,
								t_ast_node *curr, int level);
void						print_cmd_node(t_ast *ast, t_ast_node *node,
								t_ast_node *curr, int level);
void						print_red_node(t_ast *ast, t_ast_node *node,
								t_ast_node *curr, int level);

// Quit functions.

int							close_the_world(t_ast **ast);
void						exit_with_err(t_ast **ast, int err_code, char *msg);
void						exit_without_err(t_ast **ast);
int							return_with_err(int err_no, int rtn_code,
								char *msg);
int							return_prt_err(int rtn_code, char *cmd,
								char *filename, char *msg);

// Built-in functions.

bool						is_builtin_func(char *cmd);
int							exec_builtin_func(t_ast *ast, t_cmd_prop *prop);
int							cmd_echo(t_ast *ast, t_cmd_prop *prop);
int							cmd_cd(t_ast *ast, t_cmd_prop *prop);
int							cmd_pwd(t_ast *ast, t_cmd_prop *prop);
int							cmd_export(t_ast *ast, t_cmd_prop *prop);
int							cmd_unset(t_ast *ast, t_cmd_prop *prop);
int							cmd_env(t_ast *ast, t_cmd_prop *prop);
int							cmd_exit(t_ast *ast, t_cmd_prop *prop);

t_ast						*build_tree(char **tokens, int tk_size, t_env *env);

#endif