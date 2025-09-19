/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:07:16 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 04:07:46 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_args(char **mini_cmds) // eliminar a futuro
{
	int	i;

	i = 0;
	while (mini_cmds[i] != NULL)
	{
		printf("args[%d] = ( %s )\n", i, mini_cmds[i]);
		i++;
	}
	// (void)mini_cmds;
	// ft_putstr_fd("Error: command not found", STDERR_FILENO);
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	interactive_err(argc, argv);
	init_mini(&mini, argc, argv, envp);
	if (argc == 1)
		loop(&mini);
	else
		non_interactive(&mini);
	cleanup_mini(&mini);
	rl_clear_history();
	return (mini.exit_sts);
}
