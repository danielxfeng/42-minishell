// I left these functions to @Abdul, since @Abudul handles the env.

#include "../include/executor.h"

// @brief `export` with no options
int	cmd_export(t_ast *ast, t_cmd_prop *prop)
{
	(void)ast;
	(void)prop;
	return (EXIT_OK);
}

// @brief `unset` with no options
int	cmd_unset(t_ast *ast, t_cmd_prop *prop)
{
	(void)ast;
	(void)prop;
	return (EXIT_OK);
}

// @brief `env` with no options or arguments
int	cmd_env(t_ast *ast, t_cmd_prop *prop)
{
	(void)ast;
	(void)prop;
	return (EXIT_OK);
}
