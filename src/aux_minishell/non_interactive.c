/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 02:07:56 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 04:12:41 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void non_interactive(t_mini *mini) // completar con tokenización y lexer
{
	mini->input = ft_strjoin(mini->input, mini->argv[2]);
	parse(mini);
	if (mini->cmds && mini->cmds->args && built_ins(mini) == false)
		print_args(mini->cmds->args);
}
