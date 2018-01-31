#include "malloc.h"

void	*reallocf(void *ptr, size_t size)
{
	void	*new;

	new = realloc(ptr, size);
	if (!new)
		free(ptr);
	return (new);
}
