/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_travelsal_handler_supp.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:47:14 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/06 19:03:59 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/global.h"
#include "../libs/libft/libft.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int			handle_red_sig(t_red_prop *prop, char **line);
void		expand_and_write(t_ast *ast, char **line, int fd_write,
				t_red_prop *prop);

// @brief to run in a loop for reading lines from here_doc.
//
// @param ast: the pointer to ast tree.
// @param prop: the property of node.
// @param write_fd: the fd of the pipe's write endpoint.
// @return EXIT_OK.
static int	read_lines_helper(t_ast *ast, t_red_prop *prop, int fd_write)
{
	char	*line;
	char	*eof;

	eof = ast->tokens[prop->idx];
	g_here_doc_status = 0;
	sig_heredoc();
	while (true)
	{
		rl_event_hook = stop_read_line;
		line = readline("> ");
		if (handle_red_sig(prop, &line) == 130)
			return (130);
		if (!line)
			break ;
		if (ms_strcmp(eof, line) == 0)
		{
			free(line);
			line = NULL;
			break ;
		}
		expand_and_write(ast, &line, fd_write, prop);
		free(line);
	}
	sig_init();
	return (EXIT_OK);
}

// @brief handle the exit with dup
static int	exit_and_close_pipe(t_ast *ast, int *backup, int *pipe_fds,
		bool is_dup)
{
	if (backup[0] > 0)
		close(backup[0]);
	if (backup[1] > 0)
		close(backup[1]);
	if (pipe_fds[0] > 0)
		close(pipe_fds[0]);
	if (pipe_fds[1] > 0)
		close(pipe_fds[1]);
	if (is_dup)
		exit_with_err(&ast, EXIT_FAIL, "minishell: dup");
	exit_with_err(&ast, EXIT_FAIL, "minishell: dup2");
	return (EXIT_FAIL);
}

// @brief help the `red` node to handle the heredoc.
//
// @param ast: the pointer to the tree.
// @param prop: the property of the node.
// @return the status code.
int	here_doc_handler(t_ast *ast, t_red_prop *prop)
{
	int	backup[2];
	int	pipe_fds[2];

	prop->status = EXIT_FAIL;
	if (pipe(pipe_fds) < 0)
		exit_with_err(&ast, EXIT_FAIL, "minishell: pipe");
	backup[0] = dup(STDIN_FILENO);
	backup[1] = dup(STDOUT_FILENO);
	if (backup[0] == -1 || backup[1] == -1)
		exit_and_close_pipe(ast, backup, pipe_fds, true);
	if (dup2(ast->fd_in, STDIN_FILENO) < 0 || dup2(ast->fd_out,
			STDOUT_FILENO) < 0)
		exit_and_close_pipe(ast, backup, pipe_fds, false);
	read_lines_helper(ast, prop, pipe_fds[1]);
	close(pipe_fds[1]);
	prop->fd = pipe_fds[0];
	if (dup2(backup[0], STDIN_FILENO) < 0 || dup2(backup[1], STDOUT_FILENO) < 0)
		exit_and_close_pipe(ast, backup, pipe_fds, false);
	if (prop->status != 130)
		prop->status = EXIT_OK;
	return (prop->status);
}

static int	get_open_code(t_red_prop *prop)
{
	int	open_code;

	open_code = O_RDONLY;
	if (!(prop->is_in))
	{
		open_code = O_WRONLY | O_CREAT | O_TRUNC;
		if (!(prop->is_single))
			open_code = O_WRONLY | O_CREAT | O_APPEND;
	}
	return (open_code);
}

// @brief help the `red` node to open the file.
//
// @param ast: the pointer to the tree.
// @param prop: the property of the node.
// @return the status code.
int	open_file_helper(t_ast *ast, t_red_prop *prop)
{
	int		open_code;
	char	*file_name;

	open_code = get_open_code(prop);
	file_name = ast->tokens[prop->idx];
	prop->fd = open(file_name, open_code, 0644);
	if (prop->fd < 0)
	{
		prop->status = EXIT_FAIL;
		return (return_prt_err(EXIT_FAIL, "minishell", file_name, NULL));
	}
	prop->status = EXIT_OK;
	return (EXIT_OK);
}
