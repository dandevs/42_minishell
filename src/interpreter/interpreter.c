#include <stdio.h>
#include <sys/wait.h>
#include "ast.h"
#include "token_array.h"
#include "interpreter.h"


t_interpreter_result interpret(t_node *node, t_interpreter_context context)
{
	static t_interpreter_context	pid_stack[256];
	static int						pid_stack_length = 0;

	if (node->type == NODE_PIPE)
	{
		t_interpreter_context new_context = { .pipe_fd = { -1, -1 }, .is_root = 0, .non_blocking = 1 };
		t_interpreter_result result;
		int pipe_fd[2];
		pipe(pipe_fd);

		if (context.pipe_fd[0] == -1 && context.pipe_fd[1] == -1)
		{
			new_context.pipe_fd[1] = pipe_fd[1];
			interpret(node->left, new_context);
			close(pipe_fd[1]);
			new_context.pipe_fd[1] = -1;

			new_context.pipe_fd[0] = pipe_fd[0];
			result = interpret(node->right, new_context);
			close(pipe_fd[0]);
		}
		else
		{
			new_context.pipe_fd[1] = pipe_fd[1];
			interpret(node->left, new_context);
			close(pipe_fd[1]);

			new_context.pipe_fd[0] = pipe_fd[0];
			new_context.pipe_fd[1] = context.pipe_fd[1];
			result = interpret(node->right, new_context);
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
	else if (node->type == NODE_CMD)
	{
		pid_t pid = fork();
		int status;

		if (pid == 0)
		{
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
			char	*cmd;
			char	**argv;

			token_array_get_cmd(&cmd, &node->u_data.cmd.toks);
			token_array_get_args(&argv, &node->u_data.cmd.toks);

			execve(cmd, argv, NULL);
			free(argv);
			exit(127);
		}

		if (pid < 0)
		{
			return (t_interpreter_result)
			{
				.exit_status = 1,
				.signal = 0
			};
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
				return (t_interpreter_result)
				{
					.exit_status = 128 + WTERMSIG(status),
					.signal = WTERMSIG(status)
				};
			}
			else
			{
				return (t_interpreter_result)
				{
					.exit_status = WEXITSTATUS(status),
					.signal = 0
				};
			}
		}
		return (t_interpreter_result)
		{
			.exit_status = 0,
			.signal = 0
		};
	}

	if (context.is_root && pid_stack_length > 0)
	{
		int status;
		int i;

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

	return (t_interpreter_result)
	{ 
		.exit_status = 0,
		.signal = 0 
	};
}
