/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 01:59:59 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 04:08:40 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	loop(t_mini *mini)
{
	while (1)
	{
		mini->prompt = build_prompt(mini);
		if (!mini->prompt)
			mini->prompt = ft_strdup("$ ");
		mini->input = readline(mini->prompt);
		ft_signal(mini);
		if (*mini->input)
			add_history(mini->input);
		if (parse(mini) == 0 && mini->cmds)
		{
			if (expand_commands(mini) == 0)
			{
				if (built_ins(mini) == false)
				{
					if (mini->cmds->args && mini->cmds->args[0])
					{
						print_args(mini->cmds->args);
						mini->exit_sts = 127;
					}
				}
			}
		}
		cleanup_mini(mini);
	}
}
