/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 21:04:45 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/03 21:11:24 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdbool.h>
# include <sys/types.h>

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
typedef struct s_ast
{
	t_ast_node				*root;
	char					**path;
}							t_ast;

// Represents a node of AST.
// `prop`: the specific properties depends on `t_node_type`.
// `node_handler`: the pointer to handler function.
//                 This ideasimilar to a "pure virtual function" in C++,
//                 as it must be implemented differently for each node type.
// `node_closer`: the pointer to closer function. Also a "pure virtual function"
//                as well as the "destructor" in C++.
// `node_printer`: the pointer to printer function.
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
	void					(*node_printer)(t_ast_node *t_ast_node);
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