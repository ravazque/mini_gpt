/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 01:56:25 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_token	*tk_new(t_ttype type, char *raw, int sq, int dq)
{
	t_token	*tk;

	tk = (t_token *)malloc(sizeof(t_token));
	if (!tk)
		return (NULL);
	tk->raw = raw;
	tk->type = type;
	tk->had_squote = sq;
	tk->had_dquote = dq;
	tk->next = NULL;
	return (tk);
}

static void	tk_push(t_token **head, t_token *node)
{
	t_token	*cur;

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

int	is_space(int c)
{
	if (c == ' ')
		return (1);
	if (c >= 9 && c <= 13)
		return (1);
	return (0);
}

char	*ms_substr(const char *s, size_t start, size_t len)
{
	char	*out;
	size_t	i;

	out = (char *)malloc(len + 1);
	if (!out)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		out[i] = s[start + i];
		i++;
	}
	out[i] = '\0';
	return (out);
}

static int	emit_word(const char *s, size_t st, size_t ed, int sq, int dq,
		t_token **out)
{
	char	*raw;
	t_token	*tk;

	if (ed <= st)
		return (0);
	raw = ms_substr(s, st, ed - st);
	if (!raw)
		return (1);
	tk = tk_new(TK_WORD, raw, sq, dq);
	if (!tk)
	{
		free(raw);
		return (1);
	}
	tk_push(out, tk);
	return (0);
}

static int	emit_op(const char *s, size_t *i, t_token **out)
{
	if (s[*i] == '|')
	{
		tk_push(out, tk_new(TK_PIPE, ms_substr(s, *i, 1), 0, 0));
		*i = *i + 1;
		return (0);
	}
	if (s[*i] == '<')
	{
		if (s[*i + 1] == '<')
		{
			tk_push(out, tk_new(TK_HEREDOC, ms_substr(s, *i, 2), 0, 0));
			*i = *i + 2;
		}
		else
		{
			tk_push(out, tk_new(TK_REDIR_IN, ms_substr(s, *i, 1), 0, 0));
			*i = *i + 1;
		}
		return (0);
	}
	if (s[*i] == '>')
	{
		if (s[*i + 1] == '>')
		{
			tk_push(out, tk_new(TK_APPEND, ms_substr(s, *i, 2), 0, 0));
			*i = *i + 2;
		}
		else
		{
			tk_push(out, tk_new(TK_REDIR_OUT, ms_substr(s, *i, 1), 0, 0));
			*i = *i + 1;
		}
		return (0);
	}
	return (0);
}

t_token	*lex_line(const char *s, int *err)
{
	size_t	i;
	size_t	start;
	int		in_sq;
	int		in_dq;
	int		word_sq;
	int		word_dq;
	int		in_word;
	t_token	*out;

	*err = 0;
	i = 0;
	start = 0;
	in_sq = 0;
	in_dq = 0;
	word_sq = 0;
	word_dq = 0;
	in_word = 0;
	out = NULL;
	while (s[i])
	{
		if (!in_sq && !in_dq && is_space((unsigned char)s[i]))
		{
			if (in_word && emit_word(s, start, i, word_sq, word_dq, &out))
				return (*err = 1, out);
			in_word = 0;
			word_sq = 0;
			word_dq = 0;
			i++;
			continue ;
		}
		if (!in_sq && !in_dq && (s[i] == '|' || s[i] == '<' || s[i] == '>'))
		{
			if (in_word && emit_word(s, start, i, word_sq, word_dq, &out))
				return (*err = 1, out);
			in_word = 0;
			word_sq = 0;
			word_dq = 0;
			if (emit_op(s, &i, &out))
				return (*err = 1, out);
			continue ;
		}
		if (!in_dq && s[i] == '\'')
		{
			if (!in_word)
			{
				start = i;
				in_word = 1;
			}
			in_sq = !in_sq;
			word_sq = 1;
			i++;
			continue ;
		}
		if (!in_sq && s[i] == '\"')
		{
			if (!in_word)
			{
				start = i;
				in_word = 1;
			}
			in_dq = !in_dq;
			word_dq = 1;
			i++;
			continue ;
		}
		if (!in_word)
		{
			start = i;
			in_word = 1;
		}
		i++;
	}
	if (in_sq || in_dq)
	{
		*err = 1;
		return (out);
	}
	if (in_word && emit_word(s, start, i, word_sq, word_dq, &out))
	{
		*err = 1;
		return (out);
	}
	return (out);
}
