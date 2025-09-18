/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:07:16 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/18 20:48:23 by ravazque         ###   ########.fr       */
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
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	interactive_err(argc, argv);
	init_mini(&mini);
	mini.envp = envp;
	if (argc == 1)
	{
		while (1)
		{
			mini.prompt = build_prompt(&mini);
			if (!mini.prompt)
				mini.prompt = ft_strdup("$ ");
			mini.input = readline(mini.prompt);
			if (mini.input == NULL)
				break ;
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
