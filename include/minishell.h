/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:20:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 01:55:31 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/aux_libft/include/libft.h"
# include "colors.h"

# define ERR_C "Error: Argument is not -c.\n"
# define ERR_QUO "Error: syntax error\n"
# define ERR_TKN "Error: syntax error near unexpected token `"
# define ERR_C_ARG "Error: interactive mode only for now\n"

typedef struct s_token	t_token;
typedef struct s_redir	t_redir;
typedef struct s_cmd	t_cmd;
typedef struct s_mini	t_mini;

typedef enum e_ttype
{
	TK_WORD,
	TK_PIPE,
	TK_REDIR_IN,
	TK_REDIR_OUT,
	TK_APPEND,
	TK_HEREDOC
}						t_ttype;

struct					s_token
{
	char				*raw;
	t_ttype				type;
	int					had_squote;
	int					had_dquote;
	struct s_token		*next;
};

typedef enum e_rtype
{
	R_IN,
	R_OUT,
	R_APPEND,
	R_HEREDOC
}						t_rtype;

struct					s_redir
{
	t_rtype				type;
	char				*target;
	int					hd_expand;
	struct s_redir		*next;
};

struct					s_cmd
{
	char				**args;
	t_redir				*redirs;
	struct s_cmd		*next;
};

struct					s_mini
{
	char				*prompt;
	char				*input;
	char				*pwd;
	int					i;
	int					exit_sts;
	char				**env;
	t_cmd				*cmds;
};

t_token		*lex_line(const char *s, int *err);
void		free_tokens(t_token *tk);
t_cmd		*parse_tokens(t_token *tk, int *err);
int			parse(t_mini *mini);
void		free_cmds(t_cmd *cmd);
int			expand_commands(t_mini *mini);
bool		built_ins(t_mini *mini);
void		builtin_exit(t_mini *mini);
void		builtin_echo(t_mini mini);
void		builtin_pwd(t_mini *mini);
char		*build_prompt(t_mini *mini);
void		init_mini(t_mini *mini);
void		cleanup_mini(t_mini *mini);
void		free_split(char **split);
int			syntax_error(const char *near);
void		interactive_err(int argc, char *argv[]);
void		split_error(void);
void		malloc_error(void);
int			is_space(int c);
char		*ms_substr(const char *s, size_t start, size_t len);
void		builtin_env(t_mini mini);
char		**ft_copy_env(char **envp);

#endif
