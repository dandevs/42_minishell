/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 01:33:10 by mat               #+#    #+#             */
/*   Updated: 2026/06/18 15:37:25 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//start : "$USER is a fool" -> saber is a fool

/*int expand_redirs(t_ast *ast, t_redirs **redirs)
{
    int i;

    i = 0;
    while (redirs[i])
    {
        if (redirs[i]->tokens->token == DLESSER)
        {
            if (!expand_hd(redirs))
                return (0);
        }
        else
        {
            if (!expand_file(redirs))
                return (0);
        }
        i++;
    }
    return (1);
}
//add_lst_expande*/

t_list  *expand(t_ast *ast)
{
    t_list  *arg;
    t_list  *lst;
    int     i;

    while (start)
    {
        arg = ft_strdup(start->lexeme);
        if (!arg)
        {
            ft_lstclear(&lst, free);
            return (0);
        }
        ft_lstadd_back(&lst, arg);
        start = start->next;
    }
    arg = ft_lstmap(lst, env_value, free);
    ft_lstclear(&lst, free);
    if (!arg)
        return (NULL);
    return (arg);
}

int word_split(t_list *lst)
{
    t_list  *tmp;
    t_mode  mode;
    int     i;

    tmp = lst;
    while (tmp)
    {
        mode = NORMAL;
        i = 0;
        while (((char *)tmp->content)[i])
        {
            mode_change(&mode, ((char *)tmp->content)[i], i);
            if (mode == NORMAL && ft_isspace(((char *)tmp->content)[i]))
            {
                if (!ft_lstsplit(tmp, i))
                {
                    ft_lstclear(&lst, free);
                    return (0);
                }
                break ;
            }
            i++;
        }
        tmp = tmp->next;
    }
    return (1);
}
int expand_args(t_ast *ast)
{
    t_list  *lst;
    t_list  *tmp;

    lst = expand(ast);
    if (!lst)
        return (0);
    if (!word_split(lst))
        return (0);
    /*if (!remove_quotes(lst))
        return (0);
    ast->args = malloc(sizeof(char *) * (ft_lstsize(lst) + 1));
    tmp = lst;
    while (tmp)
    {
        ast->args[i] = (char *)tmp->content;
        tmp = tmp->next;
    }*/
    ft_lstclear(&lst, free);
    return (1);
}

$VAR = a b c

ls "$VAR"
"ls" "a b c"
/*  ast->args = malloc(sizeof(char) * (ft_lstsize(lst) + 1));
    ast->args[ft_lstsize(lst)] = NULL;
    i = 0;
    arg = lst;
    while (arg)
    {
        ast->args[i] = (char *)arg->content;
        arg = arg->next;
    }
    ft_lstclear(&lst, free);*/

int expand_recursive(t_ast *ast)
{
    if (ast->ast_type == AST_PIPE)
    {
        if (!expand_recursive(ast->left))
            return (0);
        if (!expand_recursive(ast->right))
            return (0);
        return (1);
    }
    if (!expand_args(ast))
        return (0);
    /*if (!expand_redirs(ast->redirs))
        return (0);*/
    return (1);
}
