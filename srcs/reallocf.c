#include "malloc.h"

FOR_EXPORT_VOID		*reallocf(void *ptr, size_t size)
{
	void	*new;

	new = realloc(ptr, size);
	if (!new)
		free(ptr);
	return (new);
}
