/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/18 20:45:09 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_redir	*rd_new(t_rtype type, char *target, int hd_expand)
{
	t_redir	*rd;

	rd = (t_redir *)malloc(sizeof(t_redir));
	if (!rd)
		return (NULL);
	rd->type = type;
	rd->target = target;
	rd->hd_expand = hd_expand;
	rd->next = NULL;
	return (rd);
}

static void	rd_push(t_redir **head, t_redir *node)
{
	t_redir	*cur;

	if (!node)
		return ;
	if (!*head)
	{
		*head = node;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

static t_cmd	*cmd_new(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

static int	args_push(char ***arr, const char *s)
{
	size_t	n;
	char	**out;

	n = 0;
	if (*arr)
	{
		while ((*arr)[n])
			n++;
	}
	out = (char **)malloc(sizeof(char *) * (n + 2));
	if (!out)
		return (1);
	n = 0;
	if (*arr)
	{
		while ((*arr)[n])
		{
			out[n] = (*arr)[n];
			n++;
		}
		free(*arr);
	}
	out[n] = ft_strdup(s);
	if (!out[n])
		return (free(out), 1);
	return (out[n + 1] = NULL, *arr = out, 0);
}

static int	parse_redir(t_token **cur, t_cmd *cmd)
{
	t_token	*op;
	t_token	*target;
	t_rtype	rtype;
	int		hd_expand;
	char	*copy;

	op = *cur;
	if (!op || (op->type != TK_REDIR_IN && op->type != TK_REDIR_OUT && op->type != TK_APPEND && op->type != TK_HEREDOC))
		return (0);
	*cur = op->next;
	target = *cur;
	if (!target || target->type != TK_WORD)
		return (syntax_error("newline"), 1);
	copy = ft_strdup(target->raw);
	if (!copy)
		return (1);
	rtype = R_IN;
	if (op->type == TK_REDIR_OUT)
		rtype = R_OUT;
	else if (op->type == TK_APPEND)
		rtype = R_APPEND;
	else if (op->type == TK_HEREDOC)
		rtype = R_HEREDOC;
	hd_expand = 1;
	if (rtype == R_HEREDOC && (target->had_squote || target->had_dquote))
		hd_expand = 0;
	rd_push(&cmd->redirs, rd_new(rtype, copy, hd_expand));
	*cur = target->next;
	return (0);
}

static int	parse_simple_cmd(t_token **cur, t_cmd *cmd)
{
	t_token	*t;

	while (*cur && ((*cur)->type == TK_REDIR_IN || (*cur)->type == TK_REDIR_OUT || (*cur)->type == TK_APPEND || (*cur)->type == TK_HEREDOC))
	{
		if (parse_redir(cur, cmd))
			return (1);
	}
	while (*cur && (*cur)->type != TK_PIPE)
	{
		t = *cur;
		if (t->type == TK_WORD)
		{
			if (args_push(&cmd->args, t->raw))
				return (1);
			*cur = t->next;
		}
		else if (t->type == TK_REDIR_IN || t->type == TK_REDIR_OUT || t->type == TK_APPEND || t->type == TK_HEREDOC)
		{
			if (parse_redir(cur, cmd))
				return (1);
		}
		else
			break ;
	}
	return (0);
}

t_cmd	*parse_tokens(t_token *tk, int *err)
{
	t_cmd	*head;
	t_cmd	*last;
	t_cmd	*cur_cmd;
	t_token	*cur;

	*err = 0;
	if (!tk)
		return (NULL);
	if (tk->type == TK_PIPE)
		return (*err = syntax_error("|"), NULL);
	head = NULL;
	last = NULL;
	cur = tk;
	while (cur)
	{
		cur_cmd = cmd_new();
		if (!cur_cmd)
			return (*err = 1, head);
		if (parse_simple_cmd(&cur, cur_cmd))
		{
			free_cmds(cur_cmd);
			return (*err = 1, head);
		}
		if (!cur_cmd->args && !cur_cmd->redirs)
		{
			free_cmds(cur_cmd);
			return (*err = syntax_error("|"), head);
		}
		if (!head)
			head = cur_cmd;
		else
			last->next = cur_cmd;
		last = cur_cmd;
		if (cur && cur->type == TK_PIPE)
		{
			cur = cur->next;
			if (!cur)
				return (*err = syntax_error("|"), head);
		}
	}
	return (head);
}
