/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 21:04:45 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/05 10:52:29 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdbool.h>
# include <sys/types.h>

# define DEBUG true
# define LEFT 0
# define RIGHT 1
# define EXIT_EXEC_ERR 126
# define EXIT_CMD_ERR 127

typedef struct s_ast_node	t_ast_node;

// Enum: type of AST nodes.
typedef enum a_node_type
{
	PIPE,
	RED,
	CMD
}							t_node_type;

// Represents the Abstract Syntax Tree as well as
// the main data structure of executor.
//
// `root` the root node of AST tree.
// `path` the string array of path from `env`.
// `tokens` the array of tokens.
// `tk_size: the size of tokens.
// `fd_in`: the std_in before executing the AST execution.
// `fd_out` the std_out before executing the AST execution.
//
// The std_in/out may be redirected in sub-process, so we need the
// original std_in/out, in case we need read/print to terminal.
typedef struct s_ast
{
	t_ast_node				*root;
	char					**path;
	char					**tokens;
	int						tk_size;
	int						fd_in;
	int						fd_out;
}							t_ast;

// Represents a node of AST.
//
// `prop`: the specific properties depends on `t_node_type`.
// `node_handler`: the pointer to handler function.
// `node_closer`: the pointer to closer function.
// `node_printer`: the pointer to printer function.
// `left` `right`: left/right child-node.
//
typedef struct s_ast_node
{
	t_node_type				type;
	void					*prop;
	int						(*node_handler)(t_ast *t_ast,
								t_ast_node *t_ast_node);
	void					(*node_closer)(t_ast_node *t_ast_node);
	void					(*node_printer)(t_ast *t_ast, t_ast_node *node,
							t_ast_node *curr, char *msg);
	t_ast_node				*left;
	t_ast_node				*right;
}							t_ast_node;

// Represents properties of CMD.
//
// `start` the index of start token for this command.
// `size` how many tokens are for this command?
// `pid` sub-process for running the `cmd`.
typedef struct s_cmd_prop
{
	int						start;
	int						size;
	pid_t					pid;
}							t_cmd_prop;

// Represents properties of RED.
//
// `idx` the index of token points to the file name.
// `fd`: the file descriptor of a file.
// `is_in`: `true` for `<` or `<<`.
// `is_single`: `true` for `<` or '>'.
typedef struct s_red_prop
{
	int						idx;
	int						fd;
	bool					is_in;
	bool					is_single;
}							t_red_prop;

// Represents properties of PIPE.
//
// `fds`: file descriptor of pipe.
// `pids`: pid of sub-process.
typedef struct s_pipe_prop
{
	int						fds[2];
	pid_t					pids[2];
}							t_pipe_prop;

// AST
// The constructors of AST.

t_ast						*create_ast(char **tokens, int tk_size,
								char **path);
t_ast_node					*create_pipe_node(t_ast *ast);
t_ast_node					*create_cmd_node(t_ast *ast, int start, int size);
t_ast_node					*create_red_node(t_ast *ast, int idx, bool is_in,
								bool is_single);

// The destructors of AST.

void						close_ast(t_ast **ast);
void						close_pipe_node(t_ast_node *node);
void						close_cmd_node(t_ast_node *node);
void						close_red_node(t_ast_node *node);

// The handlers.

int							pipe_handler(t_ast *ast, t_ast_node *ast_node);
int							cmd_handler(t_ast *ast, t_ast_node *ast_node);
int							red_handler(t_ast *ast, t_ast_node *ast_node);

// Printer of AST.

void						debug_print_ast(t_ast *ast, t_ast_node *curr,
								char *msg);
void						print_pipe_node(t_ast *ast, t_ast_node *node,
								t_ast_node *curr, char *msg);
void						print_cmd_node(t_ast *ast, t_ast_node *node,
								t_ast_node *curr, char *msg);
void						print_red_node(t_ast *ast, t_ast_node *node,
								t_ast_node *curr, char *msg);

void						exit_with_err(t_ast **ast, int err_code, char *msg);

#endif