#include <stdlib.h>
#include <string.h>
#include "token.h"

int	token_array_get_cmd(char **result, t_token_array *arr)
{
	if (arr->len < 1)
		return (0);
	*result = arr->values[0].str;
	return (1);
}

int	token_array_get_args(char ***args, t_token_array *arr)
{
	int	i;

	*args = malloc(sizeof(char *) * (arr->len + 1));
	if (!*args)
		return (0);
	i = 0;
	while (i < arr->len)
	{
		(*args)[i] = arr->values[i].str;
		i++;
	}
	(*args)[i] = NULL;
	return (1);
}