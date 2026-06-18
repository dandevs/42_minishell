#ifndef NODE_H
# define NODE_H

# include "token.h"

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE
}	t_node_type;

typedef struct s_node_cmd
{
	t_token_array toks;
}	t_node_cmd;

typedef struct s_node
{
	t_node_type		type;
	struct s_node	*left;
	struct s_node	*right;
	union
	{
		t_node_cmd cmd;
	} u_data;
}	t_node;

void node_free(t_node *node);

#endif
