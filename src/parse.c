/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/18 19:49:45 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	parse(t_mini *mini)
{
	int		err;
	t_token	*tks;
	t_cmd	*cmds;

	err = 0;
	tks = NULL;
	cmds = NULL;
	if (!mini || !mini->input)
		return (1);
	tks = lex_line(mini->input, &err);
	if (err)
	{
		free_tokens(tks);
		mini->exit_sts = 2;
		return (1);
	}
	cmds = parse_tokens(tks, &err);
	free_tokens(tks);
	if (err)
	{
		if (cmds)
			free_cmds(cmds);
		return (mini->exit_sts = 2, 1);
	}
	return (mini->cmds = cmds, 0);
}
