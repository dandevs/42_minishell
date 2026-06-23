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

void	print_redirs(t_ast *ast)
{
	int			i;

	i = 0;
	if (ast->ast_type == AST_PIPE)
	{
		print_redirs(ast->left);
		print_redirs(ast->right);
		return ;
	}
	while (ast->redirs && ast->redirs[i])
	{
		printf("redir type: %s ", ast->redirs[i]->tokens->lexeme);
		if (ast->redirs[i]->delimiter)
			printf("del: %s ", ast->redirs[i]->delimiter);
		if (ast->redirs[i]->hd)
			printf("hd: %s ", ast->redirs[i]->hd);
		if (ast->redirs[i]->file)
			printf("file: %s ", ast->redirs[i]->file->lexeme);
		printf("\n");
		i++;
	}
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

void	print_lst(t_list *lst)
{
	while (lst)
	{
		printf("%s\n", (char *)lst->content);
		lst = lst->next;
	}
}

void	print_args(t_ast *ast)
{
	if (ast->ast_type == AST_PIPE)
	{
		print_args(ast->left);
		print_args(ast->right);
		return ;
	}
	int i = 0;
	while (ast->args[i])
	{
		printf("arg[%i]:%s,", i, ast->args[i]);
		i++;
	}
	printf("\n");
}
