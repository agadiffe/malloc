#include "malloc.h"

FOR_EXPORT_VOID		*reallocf(void *ptr, size_t size)
{
	void	*new;

	pthread_mutex_init(&g_mutex, NULL);;
	pthread_mutex_lock(&g_mutex);
	new = realloc(ptr, size);
	if (!new)
		free(ptr);
	pthread_mutex_unlock(&g_mutex);
	pthread_mutex_destroy(&g_mutex);
	return (new);
}
