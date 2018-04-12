#include "malloc.h"

FOR_EXPORT_VOID		*reallocf(void *ptr, size_t size)
{
	void	*new;

	g_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&g_mutex);
	new = realloc(ptr, size);
	if (!new)
		free(ptr);
	pthread_mutex_unlock(&g_mutex);
	return (new);
}
