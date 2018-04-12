#include "malloc.h"

static void			*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)b;
	while (len)
	{
		*ptr++ = (unsigned char)c;
		len--;
	}
	return (b);
}

static void			ft_bzero(void *s, size_t n)
{
	ft_memset(s, '\0', n);
}

FOR_EXPORT_VOID		*calloc(size_t nmemb, size_t size)
{
	void	*new;

	g_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&g_mutex);
	if (nmemb && size && (new = malloc(nmemb * size)))
	{
		ft_bzero(new, ALIGN4(nmemb * size));
		return (new);
	}
	pthread_mutex_unlock(&g_mutex);
	return (NULL);
}
