/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 01:56:29 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*str_join_and_free(char *a, const char *b)
{
	size_t	la;
	size_t	lb;
	char	*out;
	size_t	i;
	size_t	j;

	la = 0;
	lb = 0;
	if (a)
	{
		while (a[la])
			la++;
	}
	if (b)
	{
		while (b[lb])
			lb++;
	}
	out = (char *)malloc(la + lb + 1);
	if (!out)
	{
		if (a)
			free(a);
		return (NULL);
	}
	i = 0;
	while (i < la)
	{
		out[i] = a[i];
		i++;
	}
	j = 0;
	while (j < lb)
	{
		out[i + j] = b[j];
		j++;
	}
	out[la + lb] = '\0';
	if (a)
		free(a);
	return (out);
}

static char	*str_append_char(char *a, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (str_join_and_free(a, tmp));
}

static int	is_varstart(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	if (c == '_')
		return (1);
	return (0);
}

static int	is_varbody(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	if (c == '_' || (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

static char	*env_lookup(t_mini *mini, const char *key)
{
	size_t	klen;
	size_t	i;
	char	*e;

	klen = 0;
	while (key[klen])
		klen++;
	if (mini && mini->env)
	{
		i = 0;
		while (mini->env[i])
		{
			if (!ft_strncmp(mini->env[i], key, klen)
				&& mini->env[i][klen] == '=')
				return (mini->env[i] + klen + 1);
			i++;
		}
	}
	e = getenv(key);
	return (e);
}

static char	*itoa_status(int n)
{
	char	buf[12];
	long	nb;
	int		i;
	int		sign;
	int		l;
	int		r;
	char	tmp;

	nb = (long)n;
	sign = 0;
	if (nb < 0)
	{
		sign = 1;
		nb = -nb;
	}
	i = 0;
	if (nb == 0)
	{
		buf[i++] = '0';
	}
	while (nb > 0)
	{
		buf[i++] = (char)('0' + (nb % 10));
		nb /= 10;
	}
	if (sign)
		buf[i++] = '-';
	buf[i] = '\0';
	{
		l = 0;
		r = i - 1;
		while (l < r)
		{
			tmp = buf[l];
			buf[l] = buf[r];
			buf[r] = tmp;
			l++;
			r--;
		}
	}
	return (ft_strdup(buf));
}

static char	*expand_word(const char *in, t_mini *mini)
{
	size_t	i;
	size_t	j;
	int		in_sq;
	int		in_dq;
	char	*out;
	char	*sts;
	char	*key;
	char	*val;

	i = 0;
	in_sq = 0;
	in_dq = 0;
	out = ft_strdup("");
	if (!out)
		return (NULL);
	while (in[i])
	{
		if (!in_dq && in[i] == '\'')
		{
			in_sq = !in_sq;
			i++;
			continue ;
		}
		if (!in_sq && in[i] == '\"')
		{
			in_dq = !in_dq;
			i++;
			continue ;
		}
		if (!in_sq && in[i] == '$')
		{
			if (in[i + 1] == '?')
			{
				sts = itoa_status(mini->exit_sts);
				if (!sts)
					return (free(out), NULL);
				out = str_join_and_free(out, sts);
				free(sts);
				i = i + 2;
				continue ;
			}
			else if (is_varstart((unsigned char)in[i + 1]))
			{
				j = i + 2;
				while (in[j] && is_varbody((unsigned char)in[j]))
					j++;
				key = ms_substr(in, i + 1, j - (i + 1));
				if (!key)
					return (free(out), NULL);
				val = (char *)env_lookup(mini, key);
				if (val)
					out = str_join_and_free(out, val);
				free(key);
				if (!out)
					return (NULL);
				i = j;
				continue ;
			}
			out = str_append_char(out, in[i]);
			if (!out)
				return (NULL);
			i++;
			continue ;
		}
		out = str_append_char(out, in[i]);
		if (!out)
			return (NULL);
		i++;
	}
	return (out);
}

static int	expand_cmd(t_cmd *cmd, t_mini *mini)
{
	size_t	i;
	char	*neww;
	t_redir	*r;

	i = 0;
	while (cmd->args && cmd->args[i])
	{
		neww = expand_word(cmd->args[i], mini);
		if (!neww)
			return (1);
		free(cmd->args[i]);
		cmd->args[i] = neww;
		i++;
	}
	r = cmd->redirs;
	while (r)
	{
		neww = expand_word(r->target, mini);
		if (!neww)
			return (1);
		free(r->target);
		r->target = neww;
		r = r->next;
	}
	return (0);
}

int	expand_commands(t_mini *mini)
{
	t_cmd	*cur;

	if (!mini || !mini->cmds)
		return (0);
	cur = mini->cmds;
	while (cur)
	{
		if (expand_cmd(cur, mini))
		{
			mini->exit_sts = 1;
			return (1);
		}
		cur = cur->next;
	}
	return (0);
}
