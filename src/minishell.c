/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:07:16 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 01:53:27 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	print_args(char **mini_cmds)
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
	init_mini(&mini);
	mini.env = ft_copy_env(envp);
	if (argc == 1)
	{
		while (1)
		{
			mini.prompt = build_prompt(&mini);
			if (!mini.prompt)
				mini.prompt = ft_strdup("$ ");
			mini.input = readline(mini.prompt);
			if (!mini.input)
			{
				write(1, "exit\n", 5) ;
				exit(0);
			}
			if (*mini.input)
				add_history(mini.input);
			if (parse(&mini) == 0 && mini.cmds)
			{
				if (expand_commands(&mini) == 0)
				{
					if (built_ins(&mini) == false)
					{
						if (mini.cmds->args && mini.cmds->args[0])
						{
							print_args(mini.cmds->args);
							mini.exit_sts = 127;
						}
					}
				}
			}
			cleanup_mini(&mini);
		}
	}
	else
	{
		mini.input = ft_strjoin(mini.input, argv[2]);
		parse(&mini);
		if (mini.cmds && mini.cmds->args && built_ins(&mini) == false)
			print_args(mini.cmds->args);
	}
	cleanup_mini(&mini);
	rl_clear_history();
	return (mini.exit_sts);
}
