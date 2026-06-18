/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 14:38:41 by marvin            #+#    #+#             */
/*   Updated: 2026/06/05 14:38:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*typedef enum	e_token
{
	PIPE,
	GREATER,
	DGREATER,
	LESSER,
	DLESSER,
	EXPAND_VAR,
	DOUBLEQ,
	SINGLEQ,
	WORD,
	END,
	NONE,
}	t_token;*/

char	*token(t_tokens *tmp)
{
	char	*type;

	type = NULL;
	if (tmp->token == PIPE)
		type = "PIPE";
	else if (tmp->token == GREATER)
		type = "GREATER";
	else if (tmp->token == DGREATER)
		type = "DGREATER";
	else if (tmp->token == LESSER)
		type = "LESSER";
	else if (tmp->token == DLESSER)
		type = "DLESSER";
	else if (tmp->token == WORD)
		type = "WORD";
	return (type);
}

void	print_tokens(t_shell *shell)
{
	t_tokens	*tmp;
	char		*type;

	tmp = shell->tokens;
	while (tmp)
	{
		type = token(tmp);
		printf("token_type: %s ; lexeme: %s\n", type, tmp->lexeme);
		tmp = tmp->next;
	}
}

void	print_cmd(char **args, t_redirs **redirs)
{
	int	i = 0;

	/*while (args && args[i])
	{
		printf("arg %i : %s; ", i, args[i]);
		i++;
	}*/
	(void)args;
	printf ("\n");
	i = 0;
	while (redirs && redirs[i])
	{
		if (redirs[i]->delimiter)
			printf("redir %i : %s delimiter ", i, redirs[i]->delimiter);
		if (redirs[i]->hd)
			printf("body = .%s.\n", redirs[i]->hd);
		else
			printf("redir %i : %s\n", i, (redirs[i])->tokens->lexeme);
		i++;
	}
	printf("\n");
}

void	print_ast(t_ast *ast)
{
	char	*type;

	if (ast->ast_type == AST_CMD)
		type = "AST_CMD";
	else
		type = "AST_PIPE";
	printf("\nast node type : %s : start == %s, ;\n", type, ast->start->lexeme);
	if (ast->end)
		printf("end == %s", ast->end->lexeme);
	if (ast->ast_type == AST_PIPE)
	{
		print_ast(ast->right);
		print_ast(ast->left);
	}
	else
	{
		print_cmd(ast->args, ast->redirs);
	}
}

void	print_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}
