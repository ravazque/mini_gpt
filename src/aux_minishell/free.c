/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 01:56:10 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_tokens(t_token *tk)
{
	t_token	*nx;

	while (tk)
	{
		nx = tk->next;
		if (tk->raw)
			free(tk->raw);
		free(tk);
		tk = nx;
	}
}

static void	free_redirs(t_redir *r)
{
	t_redir	*n;

	while (r)
	{
		n = r->next;
		if (r->target)
			free(r->target);
		free(r);
		r = n;
	}
}

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*n;
	size_t	i;

	while (cmd)
	{
		n = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		if (cmd->redirs)
			free_redirs(cmd->redirs);
		free(cmd);
		cmd = n;
	}
}

void	free_split(char **split)
{
	size_t	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	cleanup_mini(t_mini *mini)
{
	if (mini->input)
	{
		free(mini->input);
		mini->input = NULL;
	}
	if (mini->prompt)
	{
		free(mini->prompt);
		mini->prompt = NULL;
	}
	if (mini->pwd)
	{
		free(mini->pwd);
		mini->pwd = NULL;
	}
	if (mini->cmds)
	{
		free_cmds(mini->cmds);
		mini->cmds = NULL;
	}
}
