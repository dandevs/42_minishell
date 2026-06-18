#ifndef TOKEN_H
# define TOKEN_H

typedef enum e_token_type
{
	TOKEN_LITERAL,
	TOKEN_PIPE,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_IN,
}	t_token_type;

typedef struct s_token
{
	char		*str;
	int			l;
	t_token_type	type;
}	t_token;

typedef struct s_token_array
{
	int		cap;
	int		len;
	t_token	*values;
}	t_token_array;

#endif
