#ifndef INTERPRETER_H
# define INTERPRETER_H

# include "node.h"

typedef enum e_interpreter_fd_mode
{
    INTERPRETER_FD_NONE,
    INTERPRETER_FD_WRITE,
    INTERPRETER_FD_READ
}   t_interpreter_fd_mode;

typedef struct s_interpreter_result
{
    int exit_status;
    int signal;
}   t_interpreter_result;

typedef struct s_interpreter_context
{
    int is_root;
    int non_blocking;
    int pipe_fd[2];
    int pid;
}   t_interpreter_context;

t_interpreter_result    interpret(t_node *node, t_interpreter_context context);

#endif
