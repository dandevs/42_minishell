#include <stdlib.h>
#include "node.h"

void node_free(t_node *node)
{
	int	i;

	if (!node)
		return;
	node_free(node->left);
	node_free(node->right);
	if (node->type == NODE_CMD)
	{
		i = 0;
		while (i < node->u_data.cmd.toks.len)
		{
			free(node->u_data.cmd.toks.values[i].str);
			i++;
		}
		free(node->u_data.cmd.toks.values);
	}
	free(node);
}
