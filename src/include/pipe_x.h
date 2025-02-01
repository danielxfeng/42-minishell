/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_x.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 20:24:37 by Xifeng            #+#    #+#             */
/*   Updated: 2024/12/12 11:33:38 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_X_H
# define PIPE_X_H

# include <stdbool.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

# define MALLOC_ERR ": malloc() failed"
# define FORK_ERR "fork() error"
# define PIPE_ERR "pipe() creation failed"
# define PERMISSION_ERR "Permission denied"
# define FILE_ERR ": No such file or directory"
# define DIRECTORY_ERR "Is a directory"
# define DUP_ERR "dup2() error"
# define ORIGIN_DUP_ERR "dup() error"
# define CMD_ERR "command not found"
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

// The AST tree.
// `root` the root node of AST tree.
// `path` the string array of path from `env`.
// `envp` is the raw `env`.
// `fd_in` the saved `stdin`. Saving this is for `here_doc`.
// `fd_out` the saved `stdin`.
typedef struct s_ast
{
	t_ast_node				*root;
	char					**envp;
	char					**path;
	int						fd_in;
	int						fd_out;
}							t_ast;

// Represents a node of AST.
// `prop`: the specific properties depends on `t_node_type`.
// `node_handler`: the pointer to handler function.
//                 This ideasimilar to a "pure virtual function" in C++,
//                 as it must be implemented differently for each node type.
// `node_closer`: the pointer to closer function. Also a "pure virtual function"
//                as well as the "destructor" in C++.
// `left` `right`: left/right node.
//
// Note:
// - The `prop`, `node_handler`,
//	and `node_closer` fields together implement a form of
//   runtime polymorphism,
//	allowing nodes of different types (`t_node_type`) to exhibit
//   type-specific behavior.
//
//	- This idea is from the concept of polymorphism in OOP,
//   while adhering to C's procedural programming paradigm.
typedef struct s_ast_node
{
	t_node_type				type;
	void					*prop;
	int						(*node_handler)(t_ast *t_ast,
			t_ast_node *t_ast_node);
	void					(*node_closer)(t_ast_node *t_ast_node);
	t_ast_node				*left;
	t_ast_node				*right;
}							t_ast_node;

// Represents properties of CMD.
// `args` the cmd and argumens.
// `cmd` the cmd with full path.
// `pid` sub-process for running the `cmd`.
typedef struct s_cmd_prop
{
	char					**args;
	char					*full_cmd;
	pid_t					pid;
}							t_cmd_prop;

// Represents properties of RED.
// `file_name`: the filename of `RED`.
// `fd`: the file descriptor of a file.
// `is_in`: `true` for `<` or `<<`.
// `is_single`: `true` for `<` or '>'.
typedef struct s_red_prop
{
	char					*file_name;
	int						fd;
	bool					is_in;
	bool					is_single;
}							t_red_prop;

// Represents properties of PIPE.
// `fds`: file descriptor of pipe.
// `pids`: pid of sub-process.
typedef struct s_pipe_prop
{
	int						fds[2];
	pid_t					pids[2];
}							t_pipe_prop;

// AST
// The constructors of AST.

t_ast						*create_ast(char **envp, char **path);
t_ast_node					*create_pipe_node(t_ast *ast);
t_ast_node					*create_cmd_node(t_ast *ast, char *argv);
t_ast_node					*create_red_node(t_ast *ast, char *file_name,
								bool is_in, bool is_single);

// The destructors of AST.

void						close_ast(t_ast **ast);
void						close_pipe_node(t_ast_node *node);
void						close_cmd_node(t_ast_node *node);
void						close_red_node(t_ast_node *node);

// Other methods of AST.

void						print_ast(t_ast *ast);

// The handlers.

int							pipe_handler(t_ast *ast, t_ast_node *ast_node);
int							cmd_handler(t_ast *ast, t_ast_node *ast_node);
int							red_handler(t_ast *ast, t_ast_node *ast_node);
void						build_ast(t_ast *ast, int argc, char **argv,
								bool is_single);
int							return_process_res(int status);

// Parameter handler.

char						**parse_path(char **envp);
bool						validate_param(int argc, char **argv,
								bool is_bonus);

// Program.

int							pipe_x(int argc, char **argv, char **envp,
								bool is_bonus);
void						exit_prog(t_ast **ast, char *context, char *msg,
								int code);

#endif