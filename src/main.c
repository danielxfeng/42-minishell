/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 20:01:39 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/14 18:59:49 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "./include/executor.h"

void print_tokens(char **tokens);

t_shell	shell;

int	main(int ac, char **av, char **envp)
{
	return (minishell(ac, av, envp));
}

int	minishell(int ac, char **av, char **envp)
{
	t_dlist	*input;

	(void)ac;
	(void)av;
	init_minishell(&shell, envp);
	while (1)
	{
		disable_canonical_mode(&shell.termcaps);
		ft_prompt();
		input = ft_dlstnew((void *)read_input(shell.history, &shell.termcaps));
		if (!input)
			quit(EXIT_FAILURE);
		ft_dlstadd_front(&shell.history, input);
		if (!is_input_valid((const char *)input->data))
			continue ;
		enable_canonical_mode(&shell.termcaps); 
		char **tokens = tokenize((const char *)input -> data);
		print_tokens(tokens);
		int i = 0;
		while (tokens[i])
			++i;
		t_ast *tree = build_tree(tokens, i);
		tree->root->node_handler(tree, tree->root);
		close_ast(&tree);
	}
	return (0);
}
void print_tokens(char **tokens)
{
	printf("Extracted tokens:\n");
    for (int i = 0; tokens[i] != NULL; i++) {
        printf("Token %d: %s\n", i + 1, tokens[i]);
    }
}
