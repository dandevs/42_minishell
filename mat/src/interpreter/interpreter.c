/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mat <mat@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 00:00:00 by mat               #+#    #+#             */
/*   Updated: 2026/06/18 00:00:00 by mat               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

static int	is_redir(t_token t)
{
	return (t == GREATER || t == DGREATER || t == LESSER || t == DLESSER);
}

static int	count_argv(t_tokens *start, t_tokens *end)
{
	int	count;
	int	skip_next;

	count = 0;
	skip_next = 0;
	while (start && start != end)
	{
		if (skip_next)
			skip_next = 0;
		else if (is_redir(start->token))
			skip_next = 1;
		else if (start->token == WORD)
			count++;
		start = start->next;
	}
	return (count);
}

static char	**build_argv(t_tokens *start, t_tokens *end)
{
	char	**argv;
	int		i;
	int		skip_next;

	argv = malloc(sizeof(char *) * (count_argv(start, end) + 1));
	if (!argv)
		return (NULL);
	i = 0;
	skip_next = 0;
	while (start && start != end)
	{
		if (skip_next)
			skip_next = 0;
		else if (is_redir(start->token))
			skip_next = 1;
		else if (start->token == WORD)
			argv[i++] = start->lexeme;
		start = start->next;
	}
	argv[i] = NULL;
	return (argv);
}

t_interpreter_result	interpret(t_ast *ast, t_interpreter_context context)
{
	static t_interpreter_context	pid_stack[256];
	static int						pid_stack_length = 0;

	if (ast->ast_type == AST_PIPE)
	{
		t_interpreter_context	new_context;
		t_interpreter_result	result;
		int						pipe_fd[2];

		new_context = (t_interpreter_context){.pipe_fd = {-1, -1},
			.is_root = 0, .non_blocking = 1};
		pipe(pipe_fd);
		if (context.pipe_fd[0] == -1 && context.pipe_fd[1] == -1)
		{
			new_context.pipe_fd[1] = pipe_fd[1];
			interpret(ast->left, new_context);
			close(pipe_fd[1]);
			new_context.pipe_fd[1] = -1;
			new_context.pipe_fd[0] = pipe_fd[0];
			result = interpret(ast->right, new_context);
			close(pipe_fd[0]);
		}
		else
		{
			new_context.pipe_fd[1] = pipe_fd[1];
			interpret(ast->left, new_context);
			close(pipe_fd[1]);
			new_context.pipe_fd[0] = pipe_fd[0];
			new_context.pipe_fd[1] = context.pipe_fd[1];
			result = interpret(ast->right, new_context);
			close(pipe_fd[0]);
		}
		if (context.is_root && pid_stack_length > 0)
		{
			int	drain_status;
			int	drain_i;

			drain_i = 0;
			while (drain_i < pid_stack_length)
			{
				waitpid(pid_stack[drain_i].pid, &drain_status, 0);
				drain_i++;
			}
			pid_stack_length = 0;
			if (WIFSIGNALED(drain_status))
			{
				return ((t_interpreter_result)
				{
					.exit_status = 128 + WTERMSIG(drain_status),
					.signal = WTERMSIG(drain_status)
				});
			}
			return ((t_interpreter_result)
			{
				.exit_status = WEXITSTATUS(drain_status),
				.signal = 0
			});
		}
		return (result);
	}
	else if (ast->ast_type == AST_CMD)
	{
		pid_t	pid;
		int		status;

		pid = fork();
		if (pid == 0)
		{
			char	**argv;

			if (context.pipe_fd[0] != -1)
			{
				dup2(context.pipe_fd[0], STDIN_FILENO);
				close(context.pipe_fd[0]);
			}
			if (context.pipe_fd[1] != -1)
			{
				dup2(context.pipe_fd[1], STDOUT_FILENO);
				close(context.pipe_fd[1]);
			}
			argv = build_argv(ast->start, ast->end);
			if (argv && argv[0])
				execve(argv[0], argv, NULL);
			free(argv);
			exit(127);
		}
		if (pid < 0)
		{
			return ((t_interpreter_result)
			{
				.exit_status = 1,
				.signal = 0
			});
		}
		if (context.non_blocking)
		{
			context.pid = pid;
			pid_stack[pid_stack_length++] = context;
		}
		else
		{
			waitpid(pid, &status, 0);
			close(context.pipe_fd[0]);
			close(context.pipe_fd[1]);
			if (WIFSIGNALED(status))
			{
				return ((t_interpreter_result)
				{
					.exit_status = 128 + WTERMSIG(status),
					.signal = WTERMSIG(status)
				});
			}
			else
			{
				return ((t_interpreter_result)
				{
					.exit_status = WEXITSTATUS(status),
					.signal = 0
				});
			}
		}
		return ((t_interpreter_result)
		{
			.exit_status = 0,
			.signal = 0
		});
	}
	if (context.is_root && pid_stack_length > 0)
	{
		int	status;
		int	i;

		i = 0;
		while (i < pid_stack_length)
		{
			waitpid(pid_stack[i].pid, &status, 0);
			i++;
		}
		pid_stack_length = 0;
		if (WIFSIGNALED(status))
		{
			return ((t_interpreter_result)
			{
				.exit_status = 128 + WTERMSIG(status),
				.signal = WTERMSIG(status)
			});
		}
		return ((t_interpreter_result)
		{
			.exit_status = WEXITSTATUS(status),
			.signal = 0
		});
	}
	return ((t_interpreter_result)
	{
		.exit_status = 0,
		.signal = 0
	});
}
