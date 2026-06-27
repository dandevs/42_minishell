/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 18:18:57 by marvin            #+#    #+#             */
/*   Updated: 2026/04/23 18:18:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "libft.h"
# include "ft_printf.h"

extern int	g_signal;

typedef struct	s_tokens t_tokens;

typedef enum	e_mode
{
	SINGLEQ,
	DOUBLEQ,
	NORMAL
}	t_mode;

typedef enum	e_token
{
	PIPE,
	GREATER,
	DGREATER,
	LESSER,
	DLESSER,
	WORD,
	END
}	t_token;

typedef struct	s_tokens
{
	t_token		token;
	char		*lexeme;
	char		*litteral;
	t_tokens	*next;
}	t_tokens;

typedef enum e_ast_type
{
	AST_CMD,
	AST_PIPE,
}	t_ast_type;

typedef struct	s_redirs t_redirs;

typedef struct	s_redirs
{
	t_tokens	*tokens;
	t_tokens	*file;
	char		*delimiter;
	char		*hd;
}	t_redirs;

//right and left : uniquement pour noeuds de type : (AND OR) et PIPE
typedef struct	s_ast t_ast;

typedef struct	s_ast
{
	t_tokens		*start;
	t_tokens		*end;
	t_ast_type		ast_type;
	char			**args;
	t_redirs		**redirs;
	t_ast			*left;
	t_ast			*right;
}	t_ast;

typedef struct s_shell
{
	char		**envp;
	char		*line;
	t_tokens	*tokens;
	t_ast		*ast;
	int			last_status;
	char		*last_line;
}	t_shell;

typedef struct s_interpreter_result
{
	int	exit_status;
	int	signal;
}	t_interpreter_result;

typedef struct s_interpreter_context
{
	int		pid_arr[256];
	int		pid_len;
}	t_interpreter_context;

typedef int	(*t_redir_cb)(t_tokens *op, t_tokens *file);

void		setup(int agc, char **agv, char **envp, t_shell *shell);
int			check_signal(t_shell *shell, int sig);
void		setup_signals(t_shell *shell);

char		*get_line(t_shell *shell);

int			tokens_syntax_error(t_tokens *tokens);

int 		parse(t_shell *shell);

int			tokens_get(t_shell *shell, char *line);
t_tokens	*tokens_values(t_tokens *last, char *line, int i);

int			ast_build_recursive(t_ast **ast, t_tokens *start, t_tokens *end);
int			get_args(t_ast **ast, t_tokens *start, t_tokens *end);
int			get_redirs(t_ast **ast, t_tokens *start, t_tokens *end);
int			count_redirs(t_tokens *start, t_tokens *end);
int			count_args(t_tokens *start, t_tokens *end);

int			resolve_hd_recursive(t_ast *ast);

int			build_args_recursive(t_ast *ast, int last_status, char **envp);
int			expand(t_ast *ast, int last_stat, char **envp);
int			check_expand_string(char **str, int hd, char *last_status, char **envp);
int			tokens_to_lst(t_list **lst, t_tokens *start, t_tokens *end);
int			finalize_lst(t_list **lst);
int			lst_to_args(t_list *lst, t_ast *ast);

void		cleanup_and_exit(t_shell *shell, char *msg, int fd, int status);
void		cleanup_loop(t_shell *shell);
void		cleanup_shell(t_shell *shell);
void		cleanup_tokens(t_shell *shell);
void		free_ast(t_ast *ast);
void		cleanup_ast(t_shell *shell);

int 		mode_change(t_mode *mode, char *word, int i);
char		*envp_value(char *name, char *last_status, char **envp);

void		print_tokens(t_shell *shell);
void		print_ast(t_ast *ast);
void		print_envp(char **envp);
void		print_redirs(t_ast *ast);
void		print_lst(t_list *lst);
void		print_args(t_ast *ast);

t_interpreter_result	interpret(t_ast *ast);

#endif

